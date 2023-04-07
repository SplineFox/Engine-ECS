#include "ECS/Collision/CollisionSystem.h"
#include "Engine/API.h"

namespace FlatFormer { namespace Collision {

	CollisionSystem::CollisionSystem()
	{
		this->m_SystemFilter.SetItemByType<ColliderComponent>(true);
		this->m_SystemFilter.SetItemByType<ECS::TransformComponent>(true);

		m_colliders.reserve(100);// сколько резервировать?
		m_contactRegister.reserve(100);// сколько резервировать?

		m_braodPhase = new  BroadPhase();

	}


	CollisionSystem::~CollisionSystem()
	{
		auto& collisions = this->GetComponentManagerService().GetComponentsOfType<Collision::CollisionEventComponent>();

		for (size_t i = 0; i < collisions.size(); i++)
		{
			this->GetEntityManagerService().DestroyEnity(collisions[i]->GetOwnerID());
		}

		delete m_braodPhase;
	}


	//CircleCollider VS CircleCollider
	void CollisionSystem::Collide
	(
		const CircleCollider* circleA,
		ECS::EntityID entityFrom,

		const CircleCollider* circleB,
		ECS::EntityID entityTo
	)
	{
		contactInfo contInf;
	
		vec2 pA = circleA->GetWorldPosition();
		vec2 pB = circleB->GetWorldPosition();

		vec2 fromAtoB = pB - pA;

		float distance = fromAtoB.Length();

		float r = circleA->GetRadius() + circleB->GetRadius();

		if (r < distance)
		{
			return;
		}


		contInf.entityIdFrom = entityFrom;
		contInf.entityIdTo = entityTo;

		contInf.m_normal = vec2::Normalize(fromAtoB);
		contInf.m_penetrationVec = contInf.m_normal* (r - distance);
		contInf.m_contactPoints[0] = contInf.m_normal* circleA->GetRadius() + pA;
		contInf.pointCount = 1;

		contInf.m_centroidFrom = circleA->GetCentroid();
		contInf.m_centroidTo = circleB->GetCentroid();


		m_contactRegister.push_back(contInf);
	}


	//PolygonCollider VS CircleCollider
	void CollisionSystem::Collide
	(
		const PolygonCollider* polygon,
		ECS::EntityID entityFrom,
		const CircleCollider* circle,
		ECS::EntityID entityTo
	)
	{
		contactInfo contInf;

		uint8	nearestPointInex = FlatFormer::ComputeNearestPointIndex(circle->GetWorldPosition(), *polygon->GetWorldVertices());
		vec2	nearestPoint = polygon->GetWorldVertex(nearestPointInex);

		vec2Array *normals = PolygonCollider::ComputeNormals(*polygon->GetWorldVertices());
		
		vec2	allNormals[3];
		uint8	allNormalsCount = 3;

		allNormals[0] = normals->at(nearestPointInex);

		if (nearestPointInex == 0)
		{
			allNormals[1] = normals->at(normals->size() - 1);
		}
		else
		{
			allNormals[1] = normals->at(nearestPointInex - 1);
		}

		allNormals[2] = vec2::Normalize(circle->GetWorldPosition() - nearestPoint);

		vec2	normal = vec2::Zero();
		float	penetrationDepth = FFE_MAX_FLOAT;
		float	cache = 0;

		for (size_t i = 0; i < allNormalsCount; i++)
		{
			cache = FlatFormer::IntersectLength(PolygonCollider::ProjectOnNormal(*polygon->GetWorldVertices(), allNormals[i]), CircleCollider::ProjectOnNormal(circle, allNormals[i]));

			if (cache < 0)
				return;

			if (abs(penetrationDepth) > abs(cache))
			{
				penetrationDepth = cache;
				normal = allNormals[i];
			}
		}

		contInf.entityIdFrom = entityFrom;
		contInf.entityIdTo = entityTo;

		contInf.m_normal = normal;
		contInf.m_penetrationVec = normal * penetrationDepth;
		contInf.m_contactPoints[0] = circle->GetWorldPosition() + -normal * circle->GetRadius() + contInf.m_penetrationVec;
		contInf.pointCount = 1;

		contInf.m_centroidFrom = polygon->GetCentroid();
		contInf.m_centroidTo = circle->GetCentroid();

		m_contactRegister.push_back(contInf);	
	}


	//PolygonCollider VS PolygonCollider
	void CollisionSystem::Collide
	(
		const PolygonCollider* polygonA,
		ECS::EntityID entityFrom,
		const PolygonCollider* polygonB,
		ECS::EntityID entityTo
	)

	{
		contactInfo contInf;
		
		// step 1 - find the indices of the points that lie closest to the centers of the polygons
		uint8 nearestPointInexA = FlatFormer::ComputeNearestPointIndex(polygonB->GetCentroid(), *polygonA->GetWorldVertices());
		uint8 nearestPointInexB = FlatFormer::ComputeNearestPointIndex(polygonA->GetCentroid(), *polygonB->GetWorldVertices());

		vec2Array *normalsA = PolygonCollider::ComputeNormals(*polygonA->GetWorldVertices());
		vec2Array *normalsB = PolygonCollider::ComputeNormals(*polygonB->GetWorldVertices());

		// find neighboring normals to the found point
		vec2	allNormals[4];
		uint8	allNormalsACount;

		allNormals[0] = normalsA->at(nearestPointInexA);
		allNormals[1] = -normalsB->at(nearestPointInexB);

		allNormalsACount = 2;

		if (nearestPointInexA == 0)
		{
			allNormals[allNormalsACount] = normalsA->at(normalsA->size()-1);
			allNormalsACount++;
		}
		else
		{
			allNormals[allNormalsACount] = normalsA->at(nearestPointInexA - 1);
			allNormalsACount++;
		}

		if (nearestPointInexB == 0)
		{
			allNormals[allNormalsACount] = -normalsB->at(normalsB->size() - 1);
			allNormalsACount++;
		}
		else
		{
			allNormals[allNormalsACount] = -normalsB->at(nearestPointInexB - 1);
			allNormalsACount++;
		}

		// step 2 - find the normal, the penetration depth and the projection of the shapes on the found normal
		vec2	projA;
		vec2	projB;
		vec2	cacheProjA;
		vec2	cacheProjB;

		vec2	normal				= vec2::Zero();
		float	penetrationDepth	= FFE_MAX_FLOAT;
		float	cache				= 0;

		for (size_t i = 0; i < allNormalsACount; i++)
		{
			cacheProjA = PolygonCollider::ProjectOnNormal(*polygonA->GetWorldVertices(), allNormals[i]);
			cacheProjB = PolygonCollider::ProjectOnNormal(*polygonB->GetWorldVertices(), allNormals[i]);

			cache = FlatFormer::IntersectLength(cacheProjA, cacheProjB);

			if (cache < 0)
				return;

			if (abs(penetrationDepth) > abs(cache))
			{
				penetrationDepth = cache;
				normal = allNormals[i];

				projA = cacheProjA;
				projB = cacheProjB;
			}
		}

		contInf.m_normal = normal;
		contInf.m_penetrationVec = normal * penetrationDepth * FFE_PENITRATION_DEPTH_CORRECTION;

		//step 3 - get contact points
		edge edgeA = PolygonCollider::GetFaceEdge(*polygonA->GetWorldVertices(), normal);
		edge edgeB = PolygonCollider::GetFaceEdge(*polygonB->GetWorldVertices(), -normal);

		// project the penetration vector onto the normal with a slight adjustment
		float penetrProj = vec2::Dot(normal, contInf.m_penetrationVec);

		// we pull out the projection of the second shape from the intersection, onto the corrected penetration vector
		projB.x += penetrProj;
		projB.y += penetrProj;

		float vAProj1 = FlatFormer::ProjectOnNormal(edgeA.v1, normal);
		float vAProj2 = FlatFormer::ProjectOnNormal(edgeA.v2, normal);

		// projecting shifted points
		float vBProj1 = FlatFormer::ProjectOnNormal(edgeB.v1 + contInf.m_penetrationVec, normal);
		float vBProj2 = FlatFormer::ProjectOnNormal(edgeB.v2 + contInf.m_penetrationVec, normal);

		vec2Array cp1;
		cp1.reserve(2);

		vec2Array cp2;
		cp2.reserve(2);

		if (FlatFormer::Contain(projB, vAProj1))
		{
			cp1.push_back(edgeA.v1);
		}

		if (FlatFormer::Contain(projB, vAProj2))
		{
			cp1.push_back(edgeA.v2);
		}

		if (FlatFormer::Contain(projA, vBProj1))
		{
			cp2.push_back(edgeB.v1 + contInf.m_penetrationVec / FFE_PENITRATION_DEPTH_CORRECTION);
		}

		if (FlatFormer::Contain(projA, vBProj2))
		{
			cp2.push_back(edgeB.v2 + contInf.m_penetrationVec / FFE_PENITRATION_DEPTH_CORRECTION);
		}
		
		if (cp1.size() == 1)
		{
			contInf.m_contactPoints[0] = cp1[0];
			contInf.pointCount = 1;
		}
		else if (cp2.size() == 1)
		{
			contInf.m_contactPoints[0] = cp2[0];
			contInf.pointCount = 1;
		}
		else
		{
			vec2 normalA = vec2::Normalize(cp1[0] - cp1[1]);

			vec2	segments[4];
			uint8	segmentsCount = 4;

			if (cp1.size() != 0)
			{
				segments[0].x = vec2::Dot(cp1[0], normalA);
				segments[0].y = vec2::Dot(cp1[1], normalA);
				segments[1] = segments[0];
			}

			if (cp2.size() != 0)
			{
				segments[2].x = vec2::Dot(cp2[0], normalA);
				segments[2].y = vec2::Dot(cp2[1], normalA);
				segments[3] = segments[2];
			}

			

			float	pointsProj[4];
			uint8	pointsProjCount = 4;

			pointsProj[0] = segments[2].x;//pjb1
			pointsProj[1] = segments[2].y;//pjb2
			pointsProj[2] = segments[0].x;//pja1
			pointsProj[3] = segments[0].y;//pja2

			vec2	points[4];
			uint8	pointsCount = 4;

			if (cp1.size() != 0)
			{

				points[2] = cp1[0];//pa1
				points[3] = cp1[1];//pa2
			}

			if (cp2.size() != 0)
			{
				points[0] = cp2[0];//pb1
				points[1] = cp2[1];//pb2
			}
			
			contInf.pointCount = 0;

			for (uint8 i = 0; i < segmentsCount; i++)
			{
				if (FlatFormer::Contain(segments[i], pointsProj[i]) && contInf.pointCount!=2)
				{
					contInf.m_contactPoints[contInf.pointCount] = points[i];
					contInf.pointCount++;
				}
			}
		}

		contInf.m_penetrationVec /= FFE_PENITRATION_DEPTH_CORRECTION;

		contInf.entityIdFrom = entityFrom;
		contInf.entityIdTo = entityTo;

		contInf.m_centroidFrom = polygonA->GetCentroid();
		contInf.m_centroidTo = polygonB->GetCentroid();

		m_contactRegister.push_back(contInf);
	}

	//ColliderComponent VS ColliderComponent
	void CollisionSystem::Collide
	(
		const ColliderComponent* colliderA,
		ECS::EntityID entityFrom,
		const ColliderComponent* colliderB,
		ECS::EntityID entityTo
	)
	{
		switch (colliderA->m_collider->GetType())
		{

		case colliderType::circle:
		{
			if (colliderB->m_collider->GetType() == colliderType::circle)
			{
				this->Collide
				(
					(CircleCollider*)colliderA->m_collider,
					entityFrom,
					(CircleCollider*)colliderB->m_collider,
					entityTo
				);
			}
			else
			{
				this->Collide
				(
					(PolygonCollider*)colliderB->m_collider,
					entityTo,
					(CircleCollider*)colliderA->m_collider,
					entityFrom
				);
			}
			break;
		}

		case colliderType::polygon:
		{
			if (colliderB->m_collider->GetType() == colliderType::circle)
			{
				this->Collide
				(
					(PolygonCollider*)colliderA->m_collider,
					entityFrom,
					(CircleCollider*)colliderB->m_collider,
					entityTo
				);
			}
			else
			{
				this->Collide(
					(PolygonCollider*)colliderA->m_collider,
					entityFrom,
					(PolygonCollider*)colliderB->m_collider,
					entityTo
				);
			}

			break;
		}
		default:
			break;
		}
	}


	void CollisionSystem::PreUpdate(float deltaTime)
	{
		for (size_t i = 0; i < m_colliders.size(); i++)
		{
			m_colliders[i].first.first->m_collider->UpdateWorldPositions(&m_colliders[i].second->m_transform);
		}

		m_braodPhase->Update();

		m_contactRegister.clear();

		auto it = m_braodPhase->begin();
		while (it != m_braodPhase->end())
		{
			this->Collide
			(
				it->first->first,
				it->first->second,
				it->second->first,
				it->second->second
			);
			it++;
		}

		auto& collisions = this->GetComponentManagerService().GetComponentsOfType<Collision::CollisionEventComponent>();;



		for (size_t i = 0; i < m_contactRegister.size(); i++)
		{
			ColliderComponent* collider1 = this->GetComponentManagerService().GetComponent<ColliderComponent>(m_contactRegister[i].entityIdFrom);
			ColliderComponent* collider2 = this->GetComponentManagerService().GetComponent<ColliderComponent>(m_contactRegister[i].entityIdTo);

			if (!collider1->m_collider->IsTrigger() && !collider2->m_collider->IsTrigger())
			{
				this->GetEntityManagerService().CreateEntityWith<CollisionEventComponent>(&m_contactRegister[i]);
			}
		}
	}


	void CollisionSystem::Update(float deltaTime)
	{
	}


	void CollisionSystem::PostUpdate(float deltaTime)
	{
	}


	void CollisionSystem::OnEntityAdded(ECS::EntityID entityId)
	{
		ColliderComponent* collider = this->GetComponentManagerService().GetComponent<ColliderComponent>(entityId);
		ECS::TransformComponent* transform = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(entityId);

		m_colliders.emplace_back(std::pair<ColliderEntityPair, ECS::TransformComponent* >(std::make_pair(collider, entityId), transform));

		m_braodPhase->AddToPhase(new ColliderEntityPair(m_colliders.back().first.first, m_colliders.back().first.second));
	}


	void CollisionSystem::OnEntityRemoved(ECS::EntityID entityId)
	{
		ColliderComponent* collider = this->GetComponentManagerService().GetComponent<ColliderComponent>(entityId);
		ECS::TransformComponent* transform = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(entityId);

		m_braodPhase->DeleteFromPhase(&std::make_pair(collider, entityId));
		m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), std::pair<ColliderEntityPair, ECS::TransformComponent*  >(std::make_pair(collider, entityId), transform)), m_colliders.end());
	}
}}