/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef _CRT_PHYSICS_H_
#define _CRT_PHYSICS_H_ 
#ifndef NO_BULLET

#if defined SPU_BULLET || !defined (SN_TARGET_PS3)

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"

#ifdef SPU_BULLET
    #include "Physics/TaskUtil/SpursTask.h"
    #include "BulletMultiThreaded/SequentialThreadSupport.h"
    #include "Physics/TaskUtil/spurs_util_spu_printf_service.h"
    #include "SpuDispatch/btPhysicsEffectsWorld.h"
    #include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

#endif
#include "CrtRender.h"
extern CrtRender   _CrtRender;
typedef btVector3 btPoint3;

class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btDiscreteDynamicsWorld;

struct ConstraintInput;

//use some reasonable number here
#define COLLADA_CONVERTER_MAX_NUM_OBJECTS 32768

//namespace..

///ColladaConverter helps converting the physics assets from COLLADA DOM into physics objects
class ColladaConverter
{

protected:

	class DAE* m_collada;
	class domCOLLADA* m_dom;
	const char* m_filename;
	float	m_unitMeterScaling;
	
	int	m_numObjects;
	btRigidBody* m_rigidBodies[COLLADA_CONVERTER_MAX_NUM_OBJECTS];
	
	void	PreparePhysicsObject(struct btRigidBodyInput& input, bool isDynamics, btScalar mass,btCollisionShape* colShape);
	
	void	prepareConstraints(ConstraintInput& input);

	void	ConvertRigidBodyRef( struct btRigidBodyInput& , struct btRigidBodyOutput& output );


public:
	
	ColladaConverter();
	virtual ~ColladaConverter();

	///open a COLLADA .dae file
	bool	load(const char* filename);
	
	///save a snapshot in COLLADA physics .dae format.
	///if the filename is left empty, modify the filename used during loading
	bool	saveAs(const char* filename = 0);

	///convert a Collada DOM document and call the 2 virtual methods for each rigid body/constraint
	bool convert();

	///those 2 virtuals are called for each constraint/physics object
	virtual btTypedConstraint*			createUniversalD6Constraint(
		class btRigidBody* body0,class btRigidBody* otherBody,
			btTransform& localAttachmentFrameRef,
			btTransform& localAttachmentOther,
			const btVector3& linearMinLimits,
			const btVector3& linearMaxLimits,
			const btVector3& angularMinLimits,
			const btVector3& angularMaxLimits
			) = 0;

	virtual btRigidBody*  createRigidBody(const char * nodeid, bool isDynamic, 
		float mass, 
		const btTransform& startTransform,
		btCollisionShape* shape) = 0;

	virtual	void	setGravity(const CrtVec3f & vec) = 0;
	
	virtual	void	setCameraInfo(const btVector3& up, int forwardAxis) = 0;

		bool	SetColladaDOM(DAE* dae, const char * filename);

};


///custom version of the converter, that creates physics objects/constraints
class MyColladaConverter : public ColladaConverter
{
	///this is the most important class

	btCollisionDispatcher*	m_dispatcher;
	btBroadphaseInterface*	m_pairCache;
	btConstraintSolver*		m_constraintSolver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
    class btDiscreteDynamicsWorld* m_dynamicsWorld;

#ifdef SPU_BULLET
    CellSpurs mSpursInstance;
    class btThreadSupportInterface* m_collionThreadSupport;
	SampleUtilSpursPrintfService mSpursPrintfService;
#endif

	///constraint for mouse picking
	btTypedConstraint*		m_pickConstraint;
	btVector3 m_cameraUp;
	int	m_forwardAxis;

	std::map<btRigidBody *, CrtNode *> m_RigidBody_map;
	public:
		MyColladaConverter();
		virtual ~MyColladaConverter();

		///those 2 virtuals are called for each constraint/physics object
	virtual btTypedConstraint*			createUniversalD6Constraint(
		class btRigidBody* bodyRef,class btRigidBody* bodyOther,
			btTransform& localAttachmentFrameRef,
			btTransform& localAttachmentOther,
			const btVector3& linearMinLimits,
			const btVector3& linearMaxLimits,
			const btVector3& angularMinLimits,
			const btVector3& angularMaxLimits
			);

	virtual btRigidBody*  createRigidBody(const char * nodeid, bool isDynamic, 
		float mass, 
		const btTransform& startTransform,
		btCollisionShape* shape);

	virtual	void	setGravity(const CrtVec3f & vec);

	virtual void	setCameraInfo(const btVector3& camUp,int forwardAxis); 

	void Render(float delta_time);
};



#endif //SPU_BULLET
#endif //NO_BULLET  
#endif //_CRT_PHYSICS_H_  
