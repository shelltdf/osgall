/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef NO_BULLET

#include "dae.h"
#include "dom/domCOLLADA.h"
#include "dae/domAny.h"

#include "Crt/CrtPhysics.h"
#include "Crt/CrtScene.h"

#if defined SPU_BULLET || !defined (SN_TARGET_PS3)

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "LinearMath/btDefaultMotionState.h"

#ifdef SPU_BULLET

#include "SpuDispatch/btPhysicsEffectsWorld.h"//currently in Bullet/SpuDispatch
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "SpuDispatch/BulletDebugDrawer.h"
#include "BulletMultiThreaded/SequentialThreadSupport.h"
#include "BulletMultiThreaded/SequentialThreadSupport.h"
#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"

#include "SpuDispatch/BulletCollisionSpursSupport.h"
#include "SpuDispatch/BulletConstraintSolverSpursSupport.h"
#include "SpuDispatch/btPhysicsEffectsWorld.h"
#include "Physics/TaskUtil/SpursTask.h"
#include "Physics/TaskUtil/spurs_util_spu_printf_service.h"

#define SPU_THREAD_GROUP_PRIORITY	250
#define SPURS_THREAD_PRIORITY		1000
#define SPURS_NAME					"PhysicsEffects"
#define NUM_MAX_SPU                 3

#endif


bool  ColladaConverter::SetColladaDOM(DAE* dae, const char * filename)
{
	m_collada = dae;
	m_dom = m_collada->getDom(filename);
	return m_dom != NULL; 
};

char* getLastFileName();
char* fixFileName(const char* lpCmdLine);
//todo: sort out this domInstance_rigid_bodyRef forward definition, put it in the headerfile and make it virtual (make code more re-usable)

struct	btRigidBodyInput
{
	domInstance_rigid_bodyRef	m_instanceRigidBodyRef;
	domRigid_bodyRef	m_rigidBodyRef2;

	char* m_bodyName;
};

struct	ConstraintInput
{
	domInstance_physics_modelRef	m_instance_physicsModelRef;
	domPhysics_modelRef	m_model;
};


struct	btRigidBodyOutput
{
	float	m_mass;
	bool	m_isDynamics;
	btCollisionShape*	m_colShape;
	btCompoundShape*	m_compoundShape;
};



domMatrix_Array emptyMatrixArray;
domNode*	m_colladadomNodes[COLLADA_CONVERTER_MAX_NUM_OBJECTS];


///This code is actually wrong: the order of transformations is lost, so we need to rewrite this!
btTransform	GetbtTransformFromCOLLADA_DOM(domMatrix_Array& matrixArray,
														domRotate_Array& rotateArray,
														domTranslate_Array& translateArray,
														float meterScaling
														)

{
	btTransform	startTransform;
	startTransform.setIdentity();
	
	unsigned int i;
	//either load the matrix (worldspace) or incrementally build the transform from 'translate'/'rotate'
	for (i=0;i<matrixArray.getCount();i++)
	{
		domMatrixRef matrixRef = matrixArray[i];
		domFloat4x4 fl16 = matrixRef->getValue();
		btVector3 origin((btScalar)fl16.get(3), (btScalar)fl16.get(7), (btScalar)fl16.get(11));
		startTransform.setOrigin(origin*meterScaling);
		btMatrix3x3 basis((btScalar)fl16.get(0),(btScalar)fl16.get(1),(btScalar)fl16.get(2),
							(btScalar)fl16.get(4),(btScalar)fl16.get(5),(btScalar)fl16.get(6),
							(btScalar)fl16.get(8),(btScalar)fl16.get(9),(btScalar)fl16.get(10));
		startTransform.setBasis(basis);
	}

	for (i=0;i<rotateArray.getCount();i++)
	{
		domRotateRef rotateRef = rotateArray[i];
		domFloat4 fl4 = rotateRef->getValue();
		btScalar angleRad = (btScalar)SIMD_RADS_PER_DEG*(btScalar)fl4.get(3);
		btQuaternion rotQuat(btVector3((btScalar)fl4.get(0),(btScalar)fl4.get(1),(btScalar)fl4.get(2)),angleRad);
		startTransform.getBasis() = startTransform.getBasis() * btMatrix3x3(rotQuat);
	}

	for (i=0;i<translateArray.getCount();i++)
	{
		domTranslateRef translateRef = translateArray[i];
		domFloat3 fl3 = translateRef->getValue();
		btVector3 orgTrans((btScalar)fl3.get(0),(btScalar)fl3.get(1),(btScalar)fl3.get(2));
		startTransform.getOrigin() += orgTrans*meterScaling;
	}
	return startTransform;
}





ColladaConverter::ColladaConverter()
:
m_collada(0),
m_dom(0),
m_filename(0)
{
	m_numObjects = 0;
	m_unitMeterScaling = 1.0f;

	//clear 
	{

		for (int i=0;i<COLLADA_CONVERTER_MAX_NUM_OBJECTS;i++)
		{
			m_colladadomNodes[i] = 0;
		}
	}
}
	
ColladaConverter::~ColladaConverter()
{
}

bool	ColladaConverter::load(const char* orgfilename)
{

	const char* filename = fixFileName(orgfilename);
	
	int res = m_collada->load(filename);//,docBuffer);

	if (res != DAE_OK)
	{
		//some platforms might require different path, try two additional locations
		char newname[256];
		sprintf(newname,"../../%s",orgfilename);
		filename = fixFileName(newname);
		res = m_collada->load(filename);
		if (res != DAE_OK)
		{
			printf("DAE/Collada-m_dom: Couldn't load %s\n",filename);
			return false;
		}
	}
	
	if (res == DAE_OK)
	{

		m_dom = m_collada->getDom(filename);
		if ( !m_dom )
		{
			printf("COLLADA File loaded to the m_dom, but query for the m_dom assets failed \n" );
			printf("COLLADA Load Aborted! \n" );
			delete m_collada;	
			return false;
		}
	}
	return true;
}





bool ColladaConverter::convert()
{

	unsigned i;

//succesfully loaded file, now convert data

			if (m_dom->getAsset() && m_dom->getAsset()->getUnit())
			{
				domAsset::domUnitRef unit = m_dom->getAsset()->getUnit();
				domFloat meter = unit->getMeter();
				printf("asset unit meter=%f\n",meter);
//				m_unitMeterScaling = meter;		

			}
			if ( m_dom->getAsset() && m_dom->getAsset()->getUp_axis() )
			{
				domAsset::domUp_axis * up = m_dom->getAsset()->getUp_axis();
				switch( up->getValue() )
				{
				case UPAXISTYPE_X_UP:
					printf("	X is Up Data and Hiearchies must be converted!\n" ); 
					printf("  Conversion to X axis Up isn't currently supported!\n" ); 
					printf("  COLLADA_RT defaulting to Y Up \n" ); 
					setGravity(CrtVec3f(-10,0,0));
					setCameraInfo(btVector3(1,0,0),1);
					break; 
				case UPAXISTYPE_Y_UP:
					printf("	Y Axis is Up for this file \n" ); 
					printf("  COLLADA_RT set to Y Up \n" ); 
					setGravity(CrtVec3f(0,-10,0));
					setCameraInfo(btVector3(0,1,0),0);

					break;
				case UPAXISTYPE_Z_UP:
					printf("	Z Axis is Up for this file \n" ); 
					printf("  All Geometry and Hiearchies must be converted!\n" ); 
					setGravity(CrtVec3f(0,0,-10));
					break; 
				default:

					break; 
				}
			}


			//we don't handle visual objects, physics objects are rered as such
			for (unsigned int s=0;s<m_dom->getLibrary_visual_scenes_array().getCount();s++)
			{
				domLibrary_visual_scenesRef scenesRef = m_dom->getLibrary_visual_scenes_array()[s];
				for (unsigned int i=0;i<scenesRef->getVisual_scene_array().getCount();i++)
				{
					domVisual_sceneRef sceneRef = scenesRef->getVisual_scene_array()[i];
					for (unsigned int n=0;n<sceneRef->getNode_array().getCount();n++)
					{
						domNodeRef nodeRef = sceneRef->getNode_array()[n];
						nodeRef->getRotate_array();
						nodeRef->getTranslate_array();
						nodeRef->getScale_array();

					}
				}
			}




			// Load all the geometry libraries
			for (  i = 0; i < m_dom->getLibrary_geometries_array().getCount(); i++)
			{
				domLibrary_geometriesRef libgeom = m_dom->getLibrary_geometries_array()[i];

				printf(" CrtScene::Reading Geometry Library \n" );
				for ( unsigned int  i = 0; i < libgeom->getGeometry_array().getCount(); i++)
				{
					//ReadGeometry(  ); 
					domGeometryRef lib = libgeom->getGeometry_array()[i];

					domMesh			*meshElement		= lib->getMesh();
					if (meshElement)
					{
						// Find out how many groups we need to allocate space for 
						int	numTriangleGroups = (int)meshElement->getTriangles_array().getCount();
						int	numPolygonGroups  = (int)meshElement->getPolygons_array().getCount();
						int	totalGroups		  = numTriangleGroups + numPolygonGroups;
						if (totalGroups == 0) 
						{
							printf("No Triangles or Polygons found int Geometry %s \n", lib->getId() ); 
						} else
						{
							//printf("Found mesh geometry (%s): numTriangleGroups:%i numPolygonGroups:%i\n",lib->getId(),numTriangleGroups,numPolygonGroups);
						}


					}
					domConvex_mesh	*convexMeshElement	= lib->getConvex_mesh();
					if (convexMeshElement)
					{
						printf("found convexmesh element\n");
						// Find out how many groups we need to allocate space for 
						int	numTriangleGroups = (int)convexMeshElement->getTriangles_array().getCount();
						int	numPolygonGroups  = (int)convexMeshElement->getPolygons_array().getCount();

						int	totalGroups		  = numTriangleGroups + numPolygonGroups;
						if (totalGroups == 0) 
						{
							printf("No Triangles or Polygons found in ConvexMesh Geometry %s \n", lib->getId() ); 
						}else
						{
							printf("Found convexmesh geometry: numTriangleGroups:%i numPolygonGroups:%i\n",numTriangleGroups,numPolygonGroups);
						}
					}//fi
				}//for each geometry

			}//for all geometry libraries


			//m_dom->getLibrary_physics_models_array()

			for (  i = 0; i < m_dom->getLibrary_physics_scenes_array().getCount(); i++)
			{
				domLibrary_physics_scenesRef physicsScenesRef = m_dom->getLibrary_physics_scenes_array()[i];
				for (unsigned int s=0;s<physicsScenesRef->getPhysics_scene_array().getCount();s++)
				{
					domPhysics_sceneRef physicsSceneRef = physicsScenesRef->getPhysics_scene_array()[s];

					if (physicsSceneRef->getTechnique_common())
					{
						if (physicsSceneRef->getTechnique_common()->getGravity())
						{
							const domFloat3 grav = physicsSceneRef->getTechnique_common()->getGravity()->getValue();
							printf("gravity set to %f,%f,%f\n",grav.get(0),grav.get(1),grav.get(2));

							setGravity(CrtVec3f((float)grav.get(0),(float)grav.get(1),(float)grav.get(2)));
						}

					} 

					for (unsigned int ps=0;ps<physicsSceneRef->getInstance_physics_model_array().getCount();ps++)
					{
						domInstance_physics_modelRef instance_physicsModelRef = physicsSceneRef->getInstance_physics_model_array()[ps];

						daeElementRef ref = instance_physicsModelRef->getUrl().getElement();

						domPhysics_modelRef model = *(domPhysics_modelRef*)&ref; 


						unsigned int p,r;
						for ( p=0;p<model->getInstance_physics_model_array().getCount();p++)
						{
							domInstance_physics_modelRef	instancePhysicsModelRef = model->getInstance_physics_model_array()[p];

							daeElementRef ref = instancePhysicsModelRef->getUrl().getElement();
	
							domPhysics_modelRef model = *(domPhysics_modelRef*)&ref; 

							//todo: group some shared functionality in following 2 'blocks'.
							for (r=0;r<instancePhysicsModelRef->getInstance_rigid_body_array().getCount();r++)
							{
								domInstance_rigid_bodyRef instRigidbodyRef = instancePhysicsModelRef->getInstance_rigid_body_array()[r];

								btScalar mass = 1.f;
								bool isDynamics = true;
								btCollisionShape* colShape = 0;
								btCompoundShape* compoundShape = 0;

								xsNCName bodyName = instRigidbodyRef->getBody();

								domInstance_rigid_body::domTechnique_commonRef techniqueRef = instRigidbodyRef->getTechnique_common();
								if (techniqueRef)
								{
									if (techniqueRef->getMass())
									{
										mass = (btScalar)techniqueRef->getMass()->getValue();
									}
									if (techniqueRef->getDynamic())
									{
										isDynamics = techniqueRef->getDynamic()->getValue();
									}
								}


								if (bodyName && model)
								{
									//try to find the rigid body

									for (unsigned int r=0;r<model->getRigid_body_array().getCount();r++)
									{
										domRigid_bodyRef rigidBodyRef = model->getRigid_body_array()[r];
										if (rigidBodyRef->getSid() && !strcmp(rigidBodyRef->getSid(),bodyName))
										{


											btRigidBodyOutput output;
											output.m_colShape = colShape;
											output.m_compoundShape = compoundShape;
											output.m_mass = 1.f;
											output.m_isDynamics = true;

											btRigidBodyInput rbInput;
											rbInput.m_rigidBodyRef2 = rigidBodyRef;
											rbInput.m_instanceRigidBodyRef = instRigidbodyRef;
											ConvertRigidBodyRef( rbInput , output );

											mass = output.m_mass;
											isDynamics = output.m_isDynamics;
											colShape = output.m_colShape;
											compoundShape = output.m_compoundShape;

										}
									}

									//////////////////////
								}

								if (compoundShape)
									colShape = compoundShape;

								if (colShape)
								{
									btRigidBodyInput input;
									input.m_instanceRigidBodyRef = instRigidbodyRef;
									input.m_rigidBodyRef2 = 0;
									input.m_bodyName = (char*)bodyName;
									PreparePhysicsObject(input, isDynamics,mass,colShape);
								}
							}
						}


						for (r=0;r<instance_physicsModelRef->getInstance_rigid_body_array().getCount();r++)
						{

							domInstance_rigid_bodyRef instRigidbodyRef = instance_physicsModelRef->getInstance_rigid_body_array()[r];

							

							btScalar mass = 1.f;
							bool isDynamics = true;
							btCollisionShape* colShape = 0;
							btCompoundShape* compoundShape = 0;

							xsNCName bodyName = instRigidbodyRef->getBody();

							domInstance_rigid_body::domTechnique_commonRef techniqueRef = instRigidbodyRef->getTechnique_common();
							if (techniqueRef)
							{
								if (techniqueRef->getMass())
								{
									mass = (btScalar)techniqueRef->getMass()->getValue();
								}
								if (techniqueRef->getDynamic())
								{
									isDynamics = techniqueRef->getDynamic()->getValue();
								}
							}

							if (bodyName && model)
							{
								//try to find the rigid body

								for (unsigned int r=0;r<model->getRigid_body_array().getCount();r++)
								{
									domRigid_bodyRef rigidBodyRef = model->getRigid_body_array()[r];
									if (rigidBodyRef->getSid() && !strcmp(rigidBodyRef->getSid(),bodyName))
									{


										btRigidBodyOutput output;
										output.m_colShape = colShape;
										output.m_compoundShape = compoundShape;
										output.m_mass = 1.f;
										output.m_isDynamics = true;

										btRigidBodyInput rbInput;
										rbInput.m_rigidBodyRef2 = rigidBodyRef;
										rbInput.m_instanceRigidBodyRef = instRigidbodyRef;
										ConvertRigidBodyRef( rbInput , output );

										mass = output.m_mass;
										isDynamics = output.m_isDynamics;
										colShape = output.m_colShape;
										compoundShape = output.m_compoundShape;

									}
								}

								//////////////////////
							}

							if (compoundShape)
								colShape = compoundShape;

							if (colShape)
							{
								btRigidBodyInput input;
								input.m_instanceRigidBodyRef = instRigidbodyRef;
								input.m_rigidBodyRef2 = 0;
								input.m_bodyName = (char*)bodyName;
								PreparePhysicsObject(input, isDynamics,mass,colShape);
							}

						} //for  each  instance_rigid_body

						
					} //for each physics model

					
					//handle constraints
					for (unsigned int ma=0;ma<physicsSceneRef->getInstance_physics_model_array().getCount();ma++)
					{
						domInstance_physics_modelRef instance_physicsModelRef = physicsSceneRef->getInstance_physics_model_array()[ma];

						daeElementRef ref = instance_physicsModelRef->getUrl().getElement();

						domPhysics_modelRef model = *(domPhysics_modelRef*)&ref; 

						{
							ConstraintInput cInput;
							cInput.m_instance_physicsModelRef = instance_physicsModelRef;
							cInput.m_model = model;
							prepareConstraints(cInput);
						}

						//also don't forget the model's 'instance_physics_models!
						for ( unsigned int p=0;p<model->getInstance_physics_model_array().getCount();p++)
						{
							domInstance_physics_modelRef	instancePhysicsModelRef = model->getInstance_physics_model_array()[p];

							daeElementRef ref = instancePhysicsModelRef->getUrl().getElement();
	
							domPhysics_modelRef model = *(domPhysics_modelRef*)&ref; 
							
							ConstraintInput cInput;
							cInput.m_instance_physicsModelRef = instancePhysicsModelRef;
							cInput.m_model = model;
							prepareConstraints(cInput);
						}

											
					} //2nd time, for each physics model

				}
			}

			return true;
}


void	ColladaConverter::prepareConstraints(ConstraintInput& input)
{
	domInstance_physics_modelRef instance_physicsModelRef = input.m_instance_physicsModelRef;
	domPhysics_modelRef model = input.m_model;

	for (unsigned int c=0;c<instance_physicsModelRef->getInstance_rigid_constraint_array().getCount();c++)
	{
		domInstance_rigid_constraintRef constraintRef = instance_physicsModelRef->getInstance_rigid_constraint_array().get(c);
		xsNCName constraintName = constraintRef->getConstraint();

		if (constraintName && model)
		{
			//try to find the rigid body
			size_t numConstraints= model->getRigid_constraint_array().getCount();

			for (size_t r=0;r<numConstraints;r++)
			{
				domRigid_constraintRef rigidConstraintRef = model->getRigid_constraint_array()[r];
				
				if (rigidConstraintRef->getSid() && !strcmp(rigidConstraintRef->getSid(),constraintName))
				{
					
					//two bodies
					const domRigid_constraint::domRef_attachmentRef attachRefBody = rigidConstraintRef->getRef_attachment();
					const domRigid_constraint::domAttachmentRef attachBody1 = rigidConstraintRef->getAttachment();

					daeString orgUri0 = attachRefBody ? attachRefBody->getRigid_body().getOriginalURI() : "";
					daeString orgUri1 = attachBody1 ? attachBody1->getRigid_body().getOriginalURI() : "";
					btRigidBody* body0=0,*body1=0;
					
					for (int i=0;i<m_numObjects;i++)
					{
						if (m_rigidBodies[i]->getUserPointer())
						{
							char* bodyName = (char*)m_rigidBodies[i]->getUserPointer();
							if (!strcmp(bodyName,orgUri0))
							{
								body0=m_rigidBodies[i];
							}
							if (!strcmp(bodyName,orgUri1))
							{
								body1=m_rigidBodies[i];
							}
						}
					}

					const domRigid_constraint::domAttachmentRef attachOtherBody = rigidConstraintRef->getAttachment();
					const domRigid_constraint::domTechnique_commonRef commonRef = rigidConstraintRef->getTechnique_common();
					
					domFloat3 flMin, flMax, coneMinLimit, coneMaxLimit;
					btVector3 minLinearLimit, maxLinearLimit, angularMin, angularMax;
					if (commonRef)
					{
						// Check whether there is Linear Limit in COLLADA DAE fiel
						if (commonRef->getLimits()->getLinear())
						{
							flMin = commonRef->getLimits()->getLinear()->getMin()->getValue();
							minLinearLimit = btVector3((btScalar)flMin.get(0),(btScalar)flMin.get(1),(btScalar)flMin.get(2));
							flMax = commonRef->getLimits()->getLinear()->getMax()->getValue();
							maxLinearLimit = btVector3((btScalar)flMax.get(0),(btScalar)flMax.get(1),(btScalar)flMax.get(2));
						}

						// Check whether there is Swing_cone_and_twist Limit in COLLADA DAE fiel
						if (commonRef->getLimits()->getSwing_cone_and_twist())
						{
							coneMinLimit = commonRef->getLimits()->getSwing_cone_and_twist()->getMin()->getValue();
							angularMin = btVector3((btScalar)coneMinLimit.get(0),(btScalar)coneMinLimit.get(1),(btScalar)coneMinLimit.get(2));
							coneMaxLimit = commonRef->getLimits()->getSwing_cone_and_twist()->getMax()->getValue();
							angularMax = btVector3((btScalar)coneMaxLimit.get(0),(btScalar)coneMaxLimit.get(1),(btScalar)coneMaxLimit.get(2));
						}
					}

					{
						
						btTransform attachFrameRef0;
						attachFrameRef0.setIdentity();

						if (attachRefBody)
						{
							attachFrameRef0 = 
								GetbtTransformFromCOLLADA_DOM
								(
								emptyMatrixArray,
								attachRefBody->getRotate_array(),
								attachRefBody->getTranslate_array(),
								m_unitMeterScaling);
						}

						btTransform attachFrameOther;
						attachFrameOther.setIdentity();
						if (attachBody1)
						{
							attachFrameOther =
								GetbtTransformFromCOLLADA_DOM
								(
								emptyMatrixArray,
								attachBody1->getRotate_array(),
								attachBody1->getTranslate_array(),
								m_unitMeterScaling
								);
						}


						//convert INF / -INF into lower > upper

						//currently there is a hack in the DOM to detect INF / -INF
						//see daeMetaAttribute.cpp
						//INF -> 999999.9
						//-INF -> -999999.9
						float linearCheckThreshold = 999999.0;
						float angularCheckThreshold = 180.0;//check this



						
						//free means upper < lower, 
						//locked means upper == lower
						//limited means upper > lower
						//limitIndex: first 3 are linear, next 3 are angular

						btVector3 linearLowerLimits = minLinearLimit;
						btVector3 linearUpperLimits = maxLinearLimit;
						btVector3 angularLowerLimits = angularMin;
						btVector3 angularUpperLimits = angularMax;
						{
							for (int i=0;i<3;i++)
							{
								if  ((linearLowerLimits[i] < -linearCheckThreshold) ||
									(linearUpperLimits[i] > linearCheckThreshold))
								{
									//disable limits
									linearLowerLimits[i] = 1;
									linearUpperLimits[i] = 0;
								}

								if  ((angularLowerLimits[i] < -angularCheckThreshold) ||
									(angularUpperLimits[i] > angularCheckThreshold))
								{
									//disable limits
									angularLowerLimits[i] = 1;
									angularUpperLimits[i] = 0;
								}
							}
						}


						if (body0 || body1)
						{
							//swap so that first body is non-zero
							if (!body0)
							{
								createUniversalD6Constraint(
								body1,
								body0,
								attachFrameOther,
								attachFrameRef0,
								linearLowerLimits,
								linearUpperLimits,
								angularLowerLimits,
								angularUpperLimits
									);
							} else
							{
								createUniversalD6Constraint(
								body0,
								body1,
								attachFrameRef0,
								attachFrameOther,
								linearLowerLimits,
								linearUpperLimits,
								angularLowerLimits,
								angularUpperLimits
									);
							}
						} else
						{
							printf("Error: Cannot find Rigidbodies(%s,%s) for constraint %s\n",orgUri0,orgUri1,constraintName);
						}


					}



				}
			}
		}

	}

}

void	ColladaConverter::PreparePhysicsObject(struct btRigidBodyInput& input, bool isDynamics, btScalar mass,btCollisionShape* colShape)
{
	btTransform startTransform;
	startTransform.setIdentity();
	btVector3 startScale(1.f,1.f,1.f);

	//The 'target' points to a graphics element/node, which contains the start (world) transform
	daeElementRef elem = input.m_instanceRigidBodyRef->getTarget().getElement();
	if (elem)
	{
		domNodeRef node = *(domNodeRef*)&elem;
		m_colladadomNodes[m_numObjects] = node;

		//find transform of the node that this rigidbody maps to


		startTransform = GetbtTransformFromCOLLADA_DOM(
							node->getMatrix_array(),
							node->getRotate_array(),
							node->getTranslate_array(),
							m_unitMeterScaling
							);

		unsigned int i;
		for (i=0;i<node->getScale_array().getCount();i++)
		{
			domScaleRef scaleRef = node->getScale_array()[i];
			domFloat3 fl3 = scaleRef->getValue();
			startScale = btVector3((btScalar)fl3.get(0),(btScalar)fl3.get(1),(btScalar)fl3.get(2));
		}

		btRigidBody* body= createRigidBody(elem->getID(), isDynamics, (float) mass,startTransform,colShape);
		if (body)
		{
			//bodyName is used as identifier for constraints
			body->setUserPointer((void*)input.m_bodyName);
			m_rigidBodies[m_numObjects] = body;
			m_numObjects++;
		}
	}
}

bool ColladaConverter::saveAs(const char* filename)
{
	(void) filename;
	if (m_collada)
		{
			for (int i=0;i<m_numObjects;i++)
			{
				btAssert(m_colladadomNodes[i]);
				if (!m_colladadomNodes[i]->getTranslate_array().getCount())
				{
					domTranslate* transl = (domTranslate*) m_colladadomNodes[i]->createAndPlace("translate");
					transl->getValue().append(0.);
					transl->getValue().append(0.);
					transl->getValue().append(0.);
				}

				while (m_colladadomNodes[i]->getTranslate_array().getCount() > 1)
				{
					m_colladadomNodes[i]->removeFromParent(m_colladadomNodes[i]->getTranslate_array().get(1));
					//m_colladadomNodes[i]->getTranslate_array().removeIndex(1);
				}

				{

					btVector3 np = m_rigidBodies[i]->getWorldTransform().getOrigin();
					domFloat3 newPos = m_colladadomNodes[i]->getTranslate_array().get(0)->getValue();
					newPos.set(0,np[0]);
					newPos.set(1,np[1]);
					newPos.set(2,np[2]);
					m_colladadomNodes[i]->getTranslate_array().get(0)->setValue(newPos);

				}
				

				if (!m_colladadomNodes[i]->getRotate_array().getCount())
				{
					domRotate* rot = (domRotate*)m_colladadomNodes[i]->createAndPlace("rotate");
					rot->getValue().append(1.0);
					rot->getValue().append(0.0);
					rot->getValue().append(0.0);
					rot->getValue().append(0.0);
				}

				while (m_colladadomNodes[i]->getRotate_array().getCount()>1)
				{
					m_colladadomNodes[i]->removeFromParent(m_colladadomNodes[i]->getRotate_array().get(1));
					//m_colladadomNodes[i]->getRotate_array().removeIndex(1);

				}

				{
					btQuaternion quat = m_rigidBodies[i]->getCenterOfMassTransform().getRotation();
					btVector3 axis(quat.getX(),quat.getY(),quat.getZ());
					axis[3] = 0.f;
					//check for axis length
					btScalar len = axis.length2();
					if (len < SIMD_EPSILON*SIMD_EPSILON)
						axis = btVector3(1.f,0.f,0.f);
					else
						axis /= btSqrt(len);
					m_colladadomNodes[i]->getRotate_array().get(0)->getValue().set(0,axis[0]);
					m_colladadomNodes[i]->getRotate_array().get(0)->getValue().set(1,axis[1]);
					m_colladadomNodes[i]->getRotate_array().get(0)->getValue().set(2,axis[2]);
					m_colladadomNodes[i]->getRotate_array().get(0)->getValue().set(3,quat.getAngle()*SIMD_DEGS_PER_RAD);
				}

				while (m_colladadomNodes[i]->getMatrix_array().getCount())
				{
					m_colladadomNodes[i]->removeFromParent(m_colladadomNodes[i]->getMatrix_array().get(0));
					//m_colladadomNodes[i]->getMatrix_array().removeIndex(0);
				}
			}
			char	saveName[550];
			static int saveCount=1;
			sprintf(saveName,"%s%i",getLastFileName(),saveCount++);
			char* name = &saveName[0];
			if (name[0] == '/')
			{
				name = &saveName[1];
			} 
			
			if (m_dom->getAsset()->getContributor_array().getCount())
			{
				if (!m_dom->getAsset()->getContributor_array().get(0)->getAuthor())
				{
					m_dom->getAsset()->getContributor_array().get(0)->createAndPlace("author");
				}

				m_dom->getAsset()->getContributor_array().get(0)->getAuthor()->setValue
					("http://bullet.sf.net Erwin Coumans");

				if (!m_dom->getAsset()->getContributor_array().get(0)->getAuthoring_tool())
				{
					m_dom->getAsset()->getContributor_array().get(0)->createAndPlace("authoring_tool");
				}

				m_dom->getAsset()->getContributor_array().get(0)->getAuthoring_tool()->setValue
#ifdef WIN32
					("Bullet ColladaPhysicsViewer-Win32-0.8");
#else
#ifdef __APPLE__
					("Bullet ColladaPhysicsViewer-MacOSX-0.8");
#else
					("Bullet ColladaPhysicsViewer-UnknownPlatform-0.8");
#endif
#endif
				if (!m_dom->getAsset()->getContributor_array().get(0)->getComments())
				{
					m_dom->getAsset()->getContributor_array().get(0)->createAndPlace("comments");
				}
				 m_dom->getAsset()->getContributor_array().get(0)->getComments()->setValue
					 ("Comments to Physics Forum at http://www.continuousphysics.com/Bullet/phpBB2/index.php");
			}

			m_collada->saveAs(name);
			return true;
			
		}
		return false;
}

//some code that de-mangles the windows filename passed in as argument
char cleaned_filename[512];
char* getLastFileName()
{
	return cleaned_filename;
}


char* fixFileName(const char* lpCmdLine)
{


	// We might get a windows-style path on the command line, this can mess up the DOM which expects
	// all paths to be URI's.  This block of code does some conversion to try and make the input
	// compliant without breaking the ability to accept a properly formatted URI.  Right now this only
	// displays the first filename
	const char *in = lpCmdLine;
	char* out = cleaned_filename;
	*out = '\0';
	// If the first character is a ", skip it (filenames with spaces in them are quoted)
	if(*in == '\"')
	{
		in++;
	}
	if(*(in+1) == ':')
	{
		// Second character is a :, assume we have a path with a drive letter and add a slash at the beginning
		*(out++) = '/';
	}
	int i;
	for(i =0; i<512; i++)
	{
		// If we hit a null or a quote, stop copying.  This will get just the first filename.
		if(*in == '\0' || *in == '\"')
			break;
		// Copy while swapping backslashes for forward ones
		if(*in == '\\')
		{
			*out = '/';
		}
		else
		{
			*out = *in;
		}
		in++;
		out++;
	}
	
	cleaned_filename[i] = '\0'; 
	return cleaned_filename;
}


void	ColladaConverter::ConvertRigidBodyRef( btRigidBodyInput& rbInput,btRigidBodyOutput& rbOutput)
{
	
	const domRigid_body::domTechnique_commonRef techniqueRef = rbInput.m_rigidBodyRef2->getTechnique_common();
	if (techniqueRef)
	{

		if (techniqueRef->getMass())
		{
			rbOutput.m_mass = (float) techniqueRef->getMass()->getValue();
		}
		if (techniqueRef->getDynamic())
		{
			rbOutput.m_isDynamics = techniqueRef->getDynamic()->getValue();
		}
		//a hack to interpret <extra> PhysX profile:
		//when <kinematic> is true, make <dynamic> false...
		//using the DOM is a pain...
		const domExtra_Array& extraArray = rbInput.m_rigidBodyRef2->getExtra_array();
		unsigned int s=0;

		for (s = 0;s< extraArray.getCount();s++)
		{
			const domExtraRef extraRef = extraArray[s];
			const domTechnique_Array techniqueArray = extraRef->getTechnique_array();
			unsigned int t=0;
			for (t=0;t<techniqueArray.getCount();t++)
			{
				const domTechniqueRef techRef = techniqueArray[t];
				const daeElementRefArray elemRefArray = techRef->getContents();
				unsigned int u = 0;
				for (u=0;u<elemRefArray.getCount();u++)
				{
					daeElementRef elemRef = elemRefArray[u];
					daeString elemName = elemRef->getElementName();
					if (elemName && !strcmp(elemName,"kinematic"))
					{
						//how can I make this cast safe?
						const domAny* myAny = (const domAny*)elemRef.cast();
						daeString myVal = myAny->getValue();
						if (myVal)
						{
							if (!strcmp(myVal,"true"))
							{
								printf("revert bug in PhysX .dae export -> <kinematic>true</kinematic> means <dynamic>false</dynamic>\n");
								rbOutput.m_isDynamics = false;
							}
						}
					}
				}
			}
		}

		//shapes
		for (s=0;s<techniqueRef->getShape_array().getCount();s++)
		{
			domRigid_body::domTechnique_common::domShapeRef shapeRef = techniqueRef->getShape_array()[s];

			if (shapeRef->getPlane())
			{
				domPlaneRef planeRef = shapeRef->getPlane();
				if (planeRef->getEquation())
				{
					const domFloat4 planeEq = planeRef->getEquation()->getValue();
					btVector3 planeNormal((btScalar)planeEq.get(0),(btScalar)planeEq.get(1),(btScalar)planeEq.get(2));
					btScalar planeConstant = (btScalar)planeEq.get(3)*(btScalar)m_unitMeterScaling;
					rbOutput.m_colShape = new btStaticPlaneShape(planeNormal,planeConstant);
				}

			}

			if (shapeRef->getBox())
			{
				domBoxRef boxRef = shapeRef->getBox();
				domBox::domHalf_extentsRef	domHalfExtentsRef = boxRef->getHalf_extents();
				domFloat3& halfExtents = domHalfExtentsRef->getValue();
				btScalar x = (btScalar)halfExtents.get(0)*m_unitMeterScaling;
				btScalar y = (btScalar)halfExtents.get(1)*m_unitMeterScaling;
				btScalar z = (btScalar)halfExtents.get(2)*m_unitMeterScaling;
				rbOutput.m_colShape = new btBoxShape(btVector3(x,y,z));
			}
			if (shapeRef->getSphere())
			{
				domSphereRef sphereRef = shapeRef->getSphere();
				domSphere::domRadiusRef radiusRef = sphereRef->getRadius();
				btScalar radius = (btScalar)radiusRef->getValue()*m_unitMeterScaling;
				rbOutput.m_colShape = new btSphereShape(radius);
			}

			if (shapeRef->getCylinder())
			{
				domCylinderRef cylinderRef = shapeRef->getCylinder();
				domFloat height = cylinderRef->getHeight()->getValue()*m_unitMeterScaling;
				domFloat2 radius2 = cylinderRef->getRadius()->getValue();
				domFloat radius0 = radius2.get(0)*m_unitMeterScaling;

				//Cylinder around the local Y axis
				rbOutput.m_colShape = new btCylinderShapeZ(btVector3((btScalar)radius0,(btScalar)height,(btScalar)radius0));

			}

			if (shapeRef->getInstance_geometry())
			{
				const domInstance_geometryRef geomInstRef = shapeRef->getInstance_geometry();
				daeElement* geomElem = geomInstRef->getUrl().getElement();
				//elemRef->getTypeName();
				domGeometry* geom = (domGeometry*) geomElem;
				if (geom && geom->getMesh())
				{
					const domMeshRef meshRef = geom->getMesh();

					//it can be either triangle mesh, or we just pick the vertices/positions

					if (meshRef->getTriangles_array().getCount())
					{

						btTriangleMesh* trimesh = new btTriangleMesh();
						
						for (unsigned int tg = 0;tg<meshRef->getTriangles_array().getCount();tg++)
						{


							domTrianglesRef triRef = meshRef->getTriangles_array()[tg];
							const domPRef pRef = triRef->getP();
							btIndexedMesh meshPart;
							meshPart.m_triangleIndexStride=0;


							
							int vertexoffset = -1;
							domInputLocalOffsetRef indexOffsetRef;
							

							for (unsigned int w=0;w<triRef->getInput_array().getCount();w++)
							{
								domUint offset = triRef->getInput_array()[w]->getOffset();
								daeString str = triRef->getInput_array()[w]->getSemantic();
								if (!strcmp(str,"VERTEX"))
								{
									indexOffsetRef = triRef->getInput_array()[w];
									vertexoffset = (int) offset;
								}
								if ((int) offset > (int) meshPart.m_triangleIndexStride)
								{
									meshPart.m_triangleIndexStride = (int) offset;
								}
							}
							meshPart.m_triangleIndexStride++;
							domListOfUInts indexArray =triRef->getP()->getValue(); 

							//int*		m_triangleIndexBase;



							meshPart.m_numTriangles = (int) triRef->getCount();

							const domVerticesRef vertsRef = meshRef->getVertices();
							size_t numInputs = vertsRef->getInput_array().getCount();
							for (size_t i=0;i<numInputs;i++)
							{
								domInputLocalRef localRef = vertsRef->getInput_array()[i];
								daeString str = localRef->getSemantic();
								if ( !strcmp(str,"POSITION"))
								{
									domURIFragmentType& frag = localRef->getSource();

									daeElementConstRef constElem = frag.getElement();

									const domSourceRef node = *(const domSourceRef*)&constElem;
									const domFloat_arrayRef flArray = node->getFloat_array();
									if (flArray)
									{
										const domListOfFloats& listFloats = flArray->getValue();

										int k=vertexoffset;
										int t=0;
										int vertexStride = 3;//instead of hardcoded stride, should use the 'accessor'
										for (;t<meshPart.m_numTriangles;t++)
										{
											btVector3 verts[3];
											int index0;
											for (int i=0;i<3;i++)
											{
												index0 = (int) indexArray.get(k)*vertexStride;
												domFloat fl0 = listFloats.get(index0);
												domFloat fl1 = listFloats.get(index0+1);
												domFloat fl2 = listFloats.get(index0+2);
												k+=meshPart.m_triangleIndexStride;
												verts[i].setValue((btScalar)fl0*m_unitMeterScaling,(btScalar)fl1*m_unitMeterScaling,(btScalar)fl2*m_unitMeterScaling);
											}
											trimesh->addTriangle(verts[0],verts[1],verts[2]);
										}
									}
								}
							}



							if (rbOutput.m_isDynamics)
							{
								printf("moving concave <mesh> not supported, transformed into convex\n");
								rbOutput.m_colShape = new btConvexTriangleMeshShape(trimesh);
							} else
							{
								printf("static concave triangle <mesh> added\n");
								bool useQuantizedAabbCompression = false;
								rbOutput.m_colShape = new btBvhTriangleMeshShape(trimesh,useQuantizedAabbCompression);

							}

						} 
					} else
						{

							btConvexHullShape* convexHull = new btConvexHullShape();
							int numAddedVerts = 0;

							const domVerticesRef vertsRef = meshRef->getVertices();
							size_t numInputs = vertsRef->getInput_array().getCount();
							for (size_t i=0;i<numInputs;i++)
							{
								domInputLocalRef localRef = vertsRef->getInput_array()[i];
								daeString str = localRef->getSemantic();
								if ( !strcmp(str,"POSITION"))
								{
									domURIFragmentType& frag = localRef->getSource();

									daeElementConstRef constElem = frag.getElement();

									const domSourceRef node = *(const domSourceRef*)&constElem;
									const domFloat_arrayRef flArray = node->getFloat_array();
									if (flArray)
									{
										const domListOfFloats& listFloats = flArray->getValue();
										int vertexStride = 3;//instead of hardcoded stride, should use the 'accessor'
										size_t vertIndex = 0;
										for (vertIndex = 0;vertIndex < listFloats.getCount();vertIndex+=vertexStride)
										{
											domFloat fl0 = listFloats.get(vertIndex);
											domFloat fl1 = listFloats.get(vertIndex+1);
											domFloat fl2 = listFloats.get(vertIndex+2);
											convexHull->addPoint(btPoint3((btScalar)fl0,(btScalar)fl1,(btScalar)fl2) * m_unitMeterScaling);
										}
									}
								}
							}
							//convexHull->addPoint();
							if (numAddedVerts > 0)
							{
								rbOutput.m_colShape = convexHull;
							} else
							{
								delete convexHull;
								printf("no vertices found for convex hull\n");
							}

					}
						

				}

				if (geom && geom->getConvex_mesh())
				{

					{
						const domConvex_meshRef convexRef = geom->getConvex_mesh();
						daeElementRef otherElemRef = convexRef->getConvex_hull_of().getElement();
						if ( otherElemRef != NULL )
						{
							domGeometryRef linkedGeom = *(domGeometryRef*)&otherElemRef;
							printf( "otherLinked\n");
						} else
						{
							printf("convexMesh polyCount = %i\n",convexRef->getPolygons_array().getCount());
							printf("convexMesh triCount = %i\n",convexRef->getTriangles_array().getCount());

						}
					}



					btConvexHullShape* convexHullShape = new btConvexHullShape(0,0);

					//it is quite a trick to get to the vertices, using Collada.
					//we are not there yet...

					const domConvex_meshRef convexRef = geom->getConvex_mesh();
					//daeString urlref = convexRef->getConvex_hull_of().getURI();
					daeString urlref2 = convexRef->getConvex_hull_of().getOriginalURI();
					if (urlref2)
					{
						daeElementRef otherElemRef = convexRef->getConvex_hull_of().getElement();
						//	if ( otherElemRef != NULL )
						//	{
						//		domGeometryRef linkedGeom = *(domGeometryRef*)&otherElemRef;

						// Load all the geometry libraries
						for ( unsigned int i = 0; i < m_dom->getLibrary_geometries_array().getCount(); i++)
						{
							domLibrary_geometriesRef libgeom = m_dom->getLibrary_geometries_array()[i];
							//int index = libgeom->findLastIndexOf(urlref2);
							//can't find it

							for ( unsigned int  i = 0; i < libgeom->getGeometry_array().getCount(); i++)
							{
								//ReadGeometry(  ); 
								domGeometryRef lib = libgeom->getGeometry_array()[i];
								if (!strcmp(lib->getId(),urlref2))
								{
									//found convex_hull geometry
									domMesh			*meshElement		= lib->getMesh();//linkedGeom->getMesh();
									if (meshElement)
									{
										const domVerticesRef vertsRef = meshElement->getVertices();
										size_t numInputs = vertsRef->getInput_array().getCount();
										for (size_t i=0;i<numInputs;i++)
										{
											domInputLocalRef localRef = vertsRef->getInput_array()[i];
											daeString str = localRef->getSemantic();
											if ( !strcmp(str,"POSITION"))
											{
												domURIFragmentType& frag = localRef->getSource();

												daeElementConstRef constElem = frag.getElement();

												const domSourceRef node = *(const domSourceRef*)&constElem;
												const domFloat_arrayRef flArray = node->getFloat_array();
												if (flArray)
												{
													int numElem = (int) flArray->getCount();
													const domListOfFloats& listFloats = flArray->getValue();

													for (int k=0;k+2<numElem;k+=3)
													{
														domFloat fl0 = listFloats.get(k);
														domFloat fl1 = listFloats.get(k+1);
														domFloat fl2 = listFloats.get(k+2);
														//printf("float %f %f %f\n",fl0,fl1,fl2);

														convexHullShape->addPoint(btPoint3((btScalar)fl0,(btScalar)fl1,(btScalar)fl2) * m_unitMeterScaling);
													}
												}
											}
										}
									}
								}
							}
						}
					} else
					{
						//no getConvex_hull_of but direct vertices
						const domVerticesRef vertsRef = convexRef->getVertices();
						size_t numInputs = vertsRef->getInput_array().getCount();
						for (size_t i=0;i<numInputs;i++)
						{
							domInputLocalRef localRef = vertsRef->getInput_array()[i];
							daeString str = localRef->getSemantic();
							if ( !strcmp(str,"POSITION"))
							{
								domURIFragmentType& frag = localRef->getSource();

								daeElementConstRef constElem = frag.getElement();

								const domSourceRef node = *(const domSourceRef*)&constElem;
								const domFloat_arrayRef flArray = node->getFloat_array();
								if (flArray)
								{
									int numElem = (int) flArray->getCount();
									const domListOfFloats& listFloats = flArray->getValue();

									for (int k=0;k+2<numElem;k+=3)
									{
										domFloat fl0 = listFloats.get(k);
										domFloat fl1 = listFloats.get(k+1);
										domFloat fl2 = listFloats.get(k+2);
										//printf("float %f %f %f\n",fl0,fl1,fl2);

										convexHullShape->addPoint(btPoint3((btScalar)fl0,(btScalar)fl1,(btScalar)fl2)*m_unitMeterScaling);
									}

								}

							}


						}


					}

					if (convexHullShape->getNumVertices())
					{
						rbOutput.m_colShape = convexHullShape;
						printf("created convexHullShape with %i points\n",convexHullShape->getNumVertices());
					} else
					{
						delete convexHullShape;
						printf("failed to create convexHullShape\n");
					}


					//domGeometryRef linkedGeom = *(domGeometryRef*)&otherElemRef;

					printf("convexmesh\n");

				}
			}

			//if more then 1 shape, or a non-identity local shapetransform
			//use a compound

			bool hasShapeLocalTransform = ((shapeRef->getRotate_array().getCount() > 0) ||
				(shapeRef->getTranslate_array().getCount() > 0));
			
			if (rbOutput.m_colShape)
			{
				if ((techniqueRef->getShape_array().getCount()>1) ||
					(hasShapeLocalTransform))
				{
					
					if (!rbOutput.m_compoundShape)
					{
						rbOutput.m_compoundShape = new btCompoundShape();
					}

					btTransform localTransform;
					localTransform.setIdentity();
					if (hasShapeLocalTransform)
					{
					localTransform = GetbtTransformFromCOLLADA_DOM(
						emptyMatrixArray,
						shapeRef->getRotate_array(),
						shapeRef->getTranslate_array(),
						m_unitMeterScaling
						);
					}

					rbOutput.m_compoundShape->addChildShape(localTransform,rbOutput.m_colShape);
					rbOutput.m_colShape = 0;
				}
			}


		}//for each shape
	}


}


















/******************/

btRigidBody*  MyColladaConverter::createRigidBody(const char * nodeid, bool isDynamic, 
		float mass, 
		const btTransform& startTransform,
		btCollisionShape* shape)
{
	if (!isDynamic && (mass != 0.f))
	{
		printf("Warning: non-dynamic objects needs to have zero mass!\n");
		mass = 0.f;
	}

	if (isDynamic && (mass == 0.f))
	{
		printf("Warning: dynamic rigidbodies needs nonzero mass!\n");
		mass = 1.f;
	}

	isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody* body = new btRigidBody(mass,myMotionState,shape,localInertia);

	m_dynamicsWorld->addRigidBody(body);
	
	CrtScene * scene = _CrtRender.GetScene();
	if (scene) {
		CrtNode * crtnode = scene->GetNode(nodeid, NULL);
		if (crtnode) 
			m_RigidBody_map[body] = crtnode;
		crtnode->SetUpdateLocalMatrix(CrtFalse);
		crtnode->SetUpdateLocalToWorldMatrix(CrtFalse);
	}
	return body;//		btRigidBody* body = m_demoApp->localCreateRigidBody(mass, startTransform,shape);
}


MyColladaConverter::MyColladaConverter()
{
	m_cameraUp = btVector3(0,0,1);
	m_forwardAxis = 1;

  #ifdef SPU_BULLET
	// ----------------------------------------------------------
	// SPURS instance
	CellSpursAttribute	attributeSpurs;

	int ret = cellSpursAttributeInitialize (&attributeSpurs, NUM_MAX_SPU, SPU_THREAD_GROUP_PRIORITY, SPURS_THREAD_PRIORITY, false);
	if(ret) {
		printf("cellSpursAttributeInitialize failed : %x\n", ret);
		return ;
	}

	ret = cellSpursAttributeSetNamePrefix (&attributeSpurs, SPURS_NAME, strlen(SPURS_NAME));
	if(ret) {
		printf("cellSpursAttributeSetNamePrefix failed : %x\n", ret);
		return ;
	}

	ret = cellSpursInitializeWithAttribute (&mSpursInstance, &attributeSpurs);
	if(ret) {
		printf("cellSpursInitializeWithAttribute failed : %x\n", ret);
		return ;
	}

	// ----------------------------------------------------------
	// SPURS printfserver

	ret = sampleSpursUtilSpuPrintfServiceInitialize(&mSpursPrintfService, &mSpursInstance, SPURS_THREAD_PRIORITY);
	if(ret) {
		printf("spurs_printf_service_initialize failed : %d\n", ret);
		return ;
	}

  	int numSpuTasks = NUM_MAX_SPU;

    ///collision configuration contains default setup for memory, collision setup    m_collisionConfiguration = new btDefaultCollisionConfiguration();

	m_collionThreadSupport =  new BulletCollisionSpursSupport(&mSpursInstance,numSpuTasks,numSpuTasks);
    m_dispatcher = new SpuGatheringCollisionDispatcher(m_collionThreadSupport,numSpuTasks,m_collisionConfiguration);

#else
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

#endif

	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);

	m_pairCache =  new btAxisSweep3(worldMin,worldMax);
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_pairCache, m_constraintSolver, m_collisionConfiguration);

	return ;
}

MyColladaConverter::~MyColladaConverter()
{
	delete m_dynamicsWorld;
	delete m_constraintSolver;
	delete m_pairCache;
	delete m_dispatcher;
}


btTypedConstraint*	MyColladaConverter::createUniversalD6Constraint(
class btRigidBody* bodyRef,class btRigidBody* bodyOther,
	btTransform& localAttachmentFrameRef,
	btTransform& localAttachmentOther,
	const btVector3& linearMinLimits,
	const btVector3& linearMaxLimits,
	const btVector3& angularMinLimits,
	const btVector3& angularMaxLimits
	)
{
	if (bodyRef)
	{
		if (!bodyOther)
		{
			 bodyOther = new btRigidBody(0,0,0);

			 bodyOther->setWorldTransform(bodyRef->getWorldTransform());
			 localAttachmentOther = localAttachmentFrameRef;

		}
		bool useReferenceFrameA = true;
		btGeneric6DofConstraint* genericConstraint = new btGeneric6DofConstraint(
					*bodyRef,*bodyOther,
					localAttachmentFrameRef,localAttachmentOther, useReferenceFrameA);

		genericConstraint->setLinearLowerLimit(linearMinLimits);
		genericConstraint->setLinearUpperLimit(linearMaxLimits);
		genericConstraint->setAngularLowerLimit(angularMinLimits);
		genericConstraint->setAngularUpperLimit(angularMaxLimits);

		m_dynamicsWorld->addConstraint( genericConstraint );
		
		return genericConstraint;
	} 
	return 0;
}



void MyColladaConverter::Render(float delta_time)
{
//		m_dynamicsWorld->updateAabbs();

	btScalar m[16];
	
	if (m_dynamicsWorld)
	{
//		float fixedTimeStep = 1.f/60.f;
		//	if you want to test some physics simulation, uncomment out next line
		//	delta_time = fixedTimeStep;
		m_dynamicsWorld->stepSimulation(delta_time);


		int numObjects = m_dynamicsWorld->getNumCollisionObjects();
		btVector3 wireColor(1,0,0);
		for (int i=0;i<numObjects;i++)
		{
			btCollisionObject* colObj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(colObj);

			if (body && body->getMotionState())
			{
				btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
				myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
				CrtNode * crtnode = m_RigidBody_map[body];
				if (crtnode)
				{
					crtnode->SetUpdateLocalMatrix(false);
					crtnode->SetUpdateLocalToWorldMatrix(false);
					crtnode->SetLocalToWorldMatrix((CrtMatrix*) m);

//					printf("I got this node\n");
				}
			} else
			{
				colObj->getWorldTransform().getOpenGLMatrix(m);
				printf("transform matrix\n");
				printf("[ %f, %f, %f, %f]\n", m[0], m[1], m[2], m[3]);
				printf("[ %f, %f, %f, %f]\n", m[4], m[5], m[6], m[7]);
				printf("[ %f, %f, %f, %f]\n", m[8], m[9], m[10], m[11]);
				printf("[ %f, %f, %f, %f]\n", m[12], m[13], m[14], m[15]);
			}
		
		}
	}
}


void	MyColladaConverter::setGravity(const CrtVec3f & vec)
{
	m_dynamicsWorld->setGravity(btVector3(vec.x, vec.y, vec.z));
}
void	MyColladaConverter::setCameraInfo(const btVector3& camUp,int forwardAxis) 
{
	(void) camUp;
	(void) forwardAxis;
//		m_demoApp->setCameraUp(camUp);
//		m_demoApp->setCameraForwardAxis(forwardAxis);
}

#endif
#endif //NO_BULLET
