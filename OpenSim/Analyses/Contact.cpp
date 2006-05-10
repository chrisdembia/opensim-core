// Contact.cpp
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	AUTHOR: Frank C. Anderson and Saryn Goldberg
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//=============================================================================
// INCLUDES
//=============================================================================
#include <iostream>
#include <string>
#include <OpenSim/Tools/rdMath.h>
#include <OpenSim/Tools/Mtx.h>
#include <OpenSim/Tools/rdTools.h>
#include <OpenSim/Simulation/Model/Model.h>
#include "Contact.h"



using namespace OpenSim;
using namespace std;


//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/**
 * Destructor.
 */
Contact::~Contact()
{
	if(_points!=NULL) { delete[] _points;  _points=NULL; }
	if(_velocities!=NULL) { delete[] _velocities;  _velocities=NULL; }
	if(_forces!=NULL) { delete[] _forces;  _forces=NULL; }
	if(_powers!=NULL) { delete[] _powers;  _powers=NULL; }
	if(_resultantForcePoints!=NULL) {
		delete[] _resultantForcePoints;
		_resultantForcePoints=NULL;
	}
	if(_resultantForcePointGroupAssignements!=NULL) {
		delete[] _resultantForcePointGroupAssignements;
		_resultantForcePointGroupAssignements = NULL;
	}
	if(_totContactForces!=NULL) {delete[] _totContactForces;	_totContactForces=NULL; }
	if(_totContactTorques!=NULL) {delete[] _totContactTorques; _totContactTorques=NULL; }
	deleteStorage();
}
//_____________________________________________________________________________
/**
 * Construct an Contact object for recording the various qunatities
 * associates with contact.
 *
 * @param aModel Model for which the kinematics are to be recorded.
 * @param aResultantForcePointGroups Array containing group assignment for each contact point.
 *  The array must be as long as the number of contact points in the model.
 *  For a contact point to not be included in a resultant force point (RFP) calculation,
 *  assign it a negative group number.  IF all points are assigned negative numbers, no RFP
 *  analysis will be performed.
 *  If no array is given, the RFP is calcualted for all contact points in the
 *  model.
 */
Contact::Contact(Model *aModel,int *aResultantForcePointGroups) :
	Analysis(aModel)
{
	// NULL
	setNull();

	// NAME
	setName("Contact");

	// SET RESULTANT FORCE POINT VARIABLES
	int p, counter;
	_resultantForcePointGroupAssignements = aResultantForcePointGroups;
	if(_resultantForcePointGroupAssignements!=NULL){
		counter = -1;
		for(p=0;p<_model->getNP();p++){
			if(counter < _resultantForcePointGroupAssignements[p]){
				counter = _resultantForcePointGroupAssignements[p];
			}
		}
		_nResultantForcePointGroups = counter + 1;
	} else {
		_nResultantForcePointGroups = 1;
		_resultantForcePointGroupAssignements = new int[_model->getNP()];
		for(p=0;p<_model->getNP();p++)
			_resultantForcePointGroupAssignements[p]=0;
	}

	// ALLOCATE WORK ARRAYS
	int n = 6*_model->getNP();
	_points = new double[n];
	_velocities = new double[n];
	_forces = new double[n];
	_powers = new double[_model->getNP()];
	_resultantForcePoints = new double[3*_nResultantForcePointGroups];
	_totContactForces = new double[3*_nResultantForcePointGroups];
	_totContactTorques = new double[3*_nResultantForcePointGroups];

	// DESCRIPTIONS
	constructDescription();
	constructColumnLabels(_nResultantForcePointGroups);

	// STORAGE
	allocateStorage();
}


//=============================================================================
// CONSTRUCTION METHODS
//=============================================================================
//_____________________________________________________________________________
/**
 * Set NULL values for member variables.
 */
void Contact::
setNull()
{
	_points = NULL;
	_velocities = NULL;
	_forces = NULL;
	_powers = NULL;
	_resultantForcePoints = NULL;
	_totContactForces = NULL;
	_totContactTorques = NULL;
	_pStore = NULL;
	_vStore = NULL;
	_fStore = NULL;
	_pwrStore = NULL;
	_resultantForcePointsStore = NULL;
	_totFStore = NULL;
	_totTorqueStore = NULL;
	_nResultantForcePointGroups = 0;
}
//_____________________________________________________________________________
/**
 * Construct a description.
 */
void Contact::
constructDescription()
{
	string descrip = "";

	// DESCRIPTION
	descrip = "\nThis file contains the contact quantities generated ";
	descrip += "during a simulation.\n";
	descrip += "All quantities are expressed in the global frame.\n";
	descrip += "The units are S.I. (seconds, Newtons, meters, etc.).";

	// BODY ID'S
	int i,id;
	int nb = _model->getNB();
	char tmp[Object::NAME_LENGTH];
	descrip += "\n\nBody ID's:\n";
	for(i=0;i<nb;i++) {
		if(_model->getGroundID()==0) id = i+1;
		else id = i;
		sprintf(tmp,"%4d\t%s\n",id,_model->getBodyName(i).c_str());
		descrip += tmp;
	}
	descrip += "\n";

	setDescription(descrip);
}
//_____________________________________________________________________________
/**
 * Get a string of column labels.
 */
void Contact::
constructColumnLabels(int nResultantForcePointgroups)
{
	string labels = "Time";

	// VECTOR LABELS
	int nb = _model->getNB();
	int a,b,p;
	char tmp[Object::NAME_LENGTH];
	int np = _model->getNP();
	for(p=0;p<np;p++) {

		// BODY A
		a = _model->getContactBodyA(p);
		sprintf(tmp,"\t%d_A_%s_x",p,_model->getBodyName(a).c_str());
		labels += tmp;
		sprintf(tmp,"\t%d_A_%s_y",p,_model->getBodyName(a).c_str());
		labels += tmp;
		sprintf(tmp,"\t%d_A_%s_z",p,_model->getBodyName(a).c_str());
		labels += tmp;

		// BODY B
		b = _model->getContactBodyB(p);
		sprintf(tmp,"\t%d_B_%s_x",p,_model->getBodyName(b).c_str());
		labels += tmp;
		sprintf(tmp,"\t%d_B_%s_y",p,_model->getBodyName(b).c_str());
		labels += tmp;
		sprintf(tmp,"\t%d_B_%s_z",p,_model->getBodyName(b).c_str());
		labels += tmp;
	}
	setColumnLabels(labels.c_str());

	// SCALAR LABELS
	_scalarLabels = "Time";
	for(p=0;p<np;p++) {
		a = _model->getContactBodyA(p);
		b = _model->getContactBodyB(p);
		sprintf(tmp,"\t%d_%s_%s",
			p,_model->getBodyName(a).c_str(),_model->getBodyName(b).c_str());
		_scalarLabels += tmp;
	}

	// RESULTANT FORCE POINT LABELS
	_resultantForcePointLabels = "Time";
	for(p=0;p<nResultantForcePointgroups;p++) {
		sprintf(tmp,"\tresultantForcePoint_Group_%d_x",p);
		_resultantForcePointLabels = tmp;
		sprintf(tmp,"\tresultantForcePoint_Group_%d_y",p);
		_resultantForcePointLabels = tmp;
		sprintf(tmp,"\tresultantForcePoint_Group_%d_z",p);
		_resultantForcePointLabels = tmp;
	}
}
//_____________________________________________________________________________
/**
 * Allocate storage for the kinematics.
 */
void Contact::
allocateStorage()
{
	// POINTS
	_pStore = new Storage(1000,"ContactPoints");
	_pStore->setDescription(getDescription());
	_pStore->setColumnLabels(getColumnLabels());

	// VELOCITIES
	_vStore = new Storage(1000,"ContactVelocities");
	_vStore->setDescription(getDescription());
	_vStore->setColumnLabels(getColumnLabels());

	// FORCES
	_fStore = new Storage(1000,"ContactForces");
	_fStore->setDescription(getDescription());
	_fStore->setColumnLabels(getColumnLabels());

	// POWERS
	_pwrStore = new Storage(1000,"ContactPowers");
	_pwrStore->setDescription(getDescription().c_str());
	_pwrStore->setColumnLabels(getScalarColumnLabels().c_str());

	// RESULTANT FORCE POINT
	_resultantForcePointsStore = new Storage(1000,"ContactResultantForcePoint");
	_resultantForcePointsStore->setDescription(getDescription());
	_resultantForcePointsStore->setColumnLabels(getResultantForcePointColumnLabels().c_str());

	// TOTAL FORCE
	_totFStore = new Storage(1000,"ContactTotalForces");
	_totFStore->setDescription(getDescription());
	_totFStore->setColumnLabels(getResultantForcePointColumnLabels().c_str());
		
	// TOTAL TORQUE
	_totTorqueStore = new Storage(1000,"ContactTotalTorques");
	_totTorqueStore->setDescription(getDescription());
	_totTorqueStore->setColumnLabels(getResultantForcePointColumnLabels().c_str());
}


//=============================================================================
// DESTRUCTION METHODS
//=============================================================================
//_____________________________________________________________________________
/**
 * Delete storage objects.
 */
void Contact::
deleteStorage()
{
	if(_pStore!=NULL) { delete _pStore;  _pStore=NULL; }
	if(_vStore!=NULL) { delete _vStore;  _vStore=NULL; }
	if(_fStore!=NULL) { delete _fStore;  _fStore=NULL; }
	if(_pwrStore!=NULL) { delete _pwrStore;  _pwrStore=NULL; }
	if(_resultantForcePointsStore!=NULL) { delete _resultantForcePointsStore;  _resultantForcePointsStore=NULL; }
	if(_totFStore!=NULL) { delete _totFStore;  _totFStore=NULL; }
	if(_totTorqueStore!=NULL) { delete _totTorqueStore;  _totTorqueStore=NULL; }
}


//=============================================================================
// GET AND SET
//=============================================================================
//-----------------------------------------------------------------------------
// COLUMN LABELS
//-----------------------------------------------------------------------------
//_____________________________________________________________________________
/**
 * Get the column labels for scalar variables such as contact powers.
 *
 * @return Column labels for scalar variables.
 */
string Contact::
getScalarColumnLabels()
{
	return(_scalarLabels);
}
//_____________________________________________________________________________
/**
 * Get the column labels for resultant force point groups.
 *
 * @return Column labels for resultant force point groups.
 */
string Contact::
getResultantForcePointColumnLabels()
{
	return(_resultantForcePointLabels);
}

//-----------------------------------------------------------------------------
// STORAGE
//-----------------------------------------------------------------------------
//_____________________________________________________________________________
/**
 * Get the contact points storage.
 *
 * @return Points storage.
 */
Storage* Contact::
getPointsStorage() const
{
	return(_pStore);
}
//_____________________________________________________________________________
/**
 * Get the contact point velocity storage.
 *
 * @return Body-local force storage.
 */
Storage* Contact::
getVelocityStorage() const
{
	return(_vStore);
}
//_____________________________________________________________________________
/**
 * Get the force storage.
 *
 * @return Force storage.
 */
Storage* Contact::
getForceStorage() const
{
	return(_fStore);
}
//_____________________________________________________________________________
/**
 * Get the power storage.
 *
 * @return Power storage.
 */
Storage* Contact::
getPowerStorage() const
{
	return(_pwrStore);
}
//_____________________________________________________________________________
/**
 * Get the resultant force point storage.
 *
 * @return resultant force point storage.
 */
Storage* Contact::
getResultantForcePointStorage() const
{
	return(_resultantForcePointsStore);
}
//_____________________________________________________________________________
/**
 * Get the total force storage.
 *
 * @return totForce storage.
 */
Storage* Contact::
getTotForceStorage() const
{
	return(_totFStore);
}
//_____________________________________________________________________________
/**
 * Get the total torque storage.
 *
 * @return totTorque storage.
 */
Storage* Contact::
getTotTorqueStorage() const
{
	return(_totTorqueStore);
}


//-----------------------------------------------------------------------------
// STORAGE CAPACITY
//-----------------------------------------------------------------------------
//_____________________________________________________________________________
/**
 * Set the capacity increments of all storage instances.
 *
 * @param aIncrement Increment by which storage capacities will be increased
 * when storage capcities run out.
 */
void Contact::
setStorageCapacityIncrements(int aIncrement)
{
	_pStore->setCapacityIncrement(aIncrement);
	_vStore->setCapacityIncrement(aIncrement);
	_fStore->setCapacityIncrement(aIncrement);
	_pwrStore->setCapacityIncrement(aIncrement);
	_resultantForcePointsStore->setCapacityIncrement(aIncrement);
	_totFStore->setCapacityIncrement(aIncrement);
	_totTorqueStore->setCapacityIncrement(aIncrement);
}


//=============================================================================
// ANALYSIS
//=============================================================================
//_____________________________________________________________________________
/**
 * Record the contact quantities.
 */
int Contact::
record(double aT,double *aX,double *aY)
{
	double tReal = aT * _model->getTimeNormConstant();

	// SET THE STATE
	_model->set(aT,aX,aY);

	// COMPUTE CONTACT
	_model->computeContact();

	// POINTS
	const int ground = _model->getGroundID();
	int p,I,a,b,j;
	int np = _model->getNP();
	int n = 6*np;
	double vec[3];
	for(p=0;p<np;p++) {

		I = Mtx::ComputeIndex(p,6,0);

		// A
		a = _model->getContactBodyA(p);
		_model->getContactPointA(p,vec);
		_model->getPosition(a,vec,&_points[I]);

		// B
		b = _model->getContactBodyB(p);
		_model->getContactPointB(p,vec);
		_model->getPosition(b,vec,&_points[I+3]);
	}
	_pStore->append(tReal,n,_points);

	// VELOCITIES
	for(p=0;p<np;p++) {

		I = Mtx::ComputeIndex(p,6,0);

		// A
		a = _model->getContactBodyA(p);
		_model->getContactPointA(p,vec);
		_model->getVelocity(a,vec,&_velocities[I]);

		// B
		b = _model->getContactBodyB(p);
		_model->getContactPointB(p,vec);
		_model->getVelocity(b,vec,&_velocities[I+3]);
	}
	_vStore->append(tReal,n,_velocities);

	// FORCES
	for(p=0;p<np;p++) {

		I = Mtx::ComputeIndex(p,6,0);

		// FORCE ON BodyB EXPRESSED IN GROUND FRAME
		a = _model->getContactBodyA(p);
		_model->getContactForce(p,vec);
		_model->transform(a,vec,ground,vec);
		Mtx::Assign(1,3,vec,&_forces[I+3]);

		// FORCE ON BodyA IN GROUND FRAME
		Mtx::Multiply(1,3,vec,-1.0,&_forces[I]);
	}
	_fStore->append(tReal,n,_forces);

	// POWERS
	for(p=0;p<np;p++) {
		_powers[p] = _model->getContactPower(p);
	}
	_pwrStore->append(tReal,_model->getNP(),_powers);

	//RESULTANT FORCE POINT
	int g, index;
	int bodyA, bodyB;
	double resultantForcePoint[3];
	double totContactForce[3];
	double totContactTorque[3];
	double contactForce[3];
	double contactPosRelCOMLocal[3];
	double contactPosRelCOMGlobal[3];
	//double contactPosGlobal[3];
	double posBodyCOMLocal[3] = {0,0,0};
	//double posBodyCOMGlobal[3];
	double contactTorque[3];

	for(g=0;g<_nResultantForcePointGroups;g++){
		for(j=0;j<3;j++){
			 totContactForce[j]=0.0;
			 totContactTorque[j]=0.0;
		}
		for(p=0;p<np;p++){
			if(_resultantForcePointGroupAssignements[p]==g){
				bodyA = _model->getContactBodyA(p);
				bodyB = _model->getContactBodyB(p);
				_model->getContactForce(p,contactForce);
				_model->transform(bodyA,contactForce,ground,contactForce);
				Mtx::Add(1,3,totContactForce,contactForce,totContactForce);
				_model->getContactPointB(p, contactPosRelCOMLocal);

				_model->transformPosition(bodyB,contactPosRelCOMLocal,contactPosRelCOMGlobal);

//				_model->getPosition(bodyB,contactPosRelCOMLocal,contactPosGlobal);
//				_model->getPosition(bodyB,posBodyCOMLocal,posBodyCOMGlobal);
//				Mtx::Subtract(1,3,contactPosGlobal,posBodyCOMGlobal,contactPosRelCOMGlobal);

				Mtx::CrossProduct(contactPosRelCOMGlobal,contactForce,contactTorque);
				Mtx::Add(1,3,contactTorque,totContactTorque,totContactTorque);
			}
		}
		if(fabs(totContactForce[1])>rdMath::ZERO) {
			resultantForcePoint[0] = totContactTorque[2]/totContactForce[1];
			resultantForcePoint[1] = rdMath::NAN;
			resultantForcePoint[2] = -totContactTorque[0]/totContactForce[1];
		} else {
			for(j=0;j<3;j++){
				resultantForcePoint[j] = rdMath::NAN;
			}
		}
		for(j=0;j<3;j++){
			index = Mtx::ComputeIndex(g,3,j);
			_resultantForcePoints[index] = resultantForcePoint[j];
			_totContactForces[index] = totContactForce[j];
			_totContactTorques[index] = totContactTorque[j];

		}
	}
	if(_nResultantForcePointGroups>0)
		_resultantForcePointsStore->append(tReal,3*_nResultantForcePointGroups,_resultantForcePoints);
		_totFStore->append(tReal,3*_nResultantForcePointGroups,_totContactForces);
		_totTorqueStore->append(tReal,3*_nResultantForcePointGroups,_totContactTorques);


	return(0);
}
//_____________________________________________________________________________
/**
 * This method is called at the beginning of an analysis so that any
 * necessary initializations may be performed.
 *
 * This method is meant to be called at the begining of an integration in
 * Model::integBeginCallback() and has the same argument list.
 *
 * This method should be overriden in the child class.  It is
 * included here so that the child class will not have to implement it if it
 * is not necessary.
 *
 * @param aStep Step number of the integration.
 * @param aDT Size of the time step that will be attempted.
 * @param aT Current time in the integration.
 * @param aX Current control values.
 * @param aY Current states.
 * @param aClientData General use pointer for sending in client data.
 *
 * @return -1 on error, 0 otherwise.
 */
int Contact::
begin(int aStep,double aDT,double aT,double *aX,double *aY,
		void *aClientData)
{
	if(!proceed()) return(0);

	// RESET STORAGE
	_pStore->reset(aT);
	_vStore->reset(aT);
	_fStore->reset(aT);
	_pwrStore->reset(aT);
	_resultantForcePointsStore->reset(aT);
	_totFStore->reset(aT);
	_totTorqueStore->reset(aT);



	// RECORD
	int status = 0;
	if(_pStore->getSize()<=0) {
		status = record(aT,aX,aY);
	}

	return(status);
}
//_____________________________________________________________________________
/**
 * This method is called to perform the analysis.  It can be called during
 * the execution of a forward integrations or after the integration by
 * feeding it the necessary data.
 *
 * When called during an integration, this method is meant to be called in
 * Model::integStepCallback(), which has the same argument list.
 *
 * This method should be overriden in derived classes.  It is
 * included here so that the derived class will not have to implement it if
 * it is not necessary.
 *
 * @param aXPrev Controls at the beginining of the current time step.
 * @param aYPrev States at the beginning of the current time step.
 * @param aStep Step number of the integration.
 * @param aDT Size of the time step that was just taken.
 * @param aT Current time in the integration.
 * @param aX Current control values.
 * @param aY Current states.
 * @param aClientData General use pointer for sending in client data.
 *
 * @return -1 on error, 0 otherwise.
 */
int Contact::
step(double *aXPrev,double *aYPrev,
	int aStep,double aDT,double aT,double *aX,double *aY,
	void *aClientData)
{
	if(!proceed(aStep)) return(0);

	int status = record(aT,aX,aY);

	return(0);
}
//_____________________________________________________________________________
/**
 * This method is called at the end of an analysis so that any
 * necessary finalizations may be performed.
 *
 * This method is meant to be called at the end of an integration in
 * Model::integEndCallback() and has the same argument list.
 *
 * This method should be overriden in the child class.  It is
 * included here so that the child class will not have to implement it if it
 * is not necessary.
 *
 * @param aStep Step number of the integration.
 * @param aDT Size of the time step that was just completed.
 * @param aT Current time in the integration.
 * @param aX Current control values.
 * @param aY Current states.
 * @param aClientData General use pointer for sending in client data.
 *
 * @return -1 on error, 0 otherwise.
 */
int Contact::
end(int aStep,double aDT,double aT,double *aX,double *aY,
		void *aClientData)
{
	return(0);
}


//=============================================================================
// UTILITIES
//=============================================================================
//_____________________________________________________________________________
/**
 * Reset all storage objects.  That is, delete all internally stored
 * data so that new data may be stored.
 */
void Contact::
resetStorage()
{
	if(_pStore!=NULL) _pStore->reset();
	if(_vStore!=NULL) _vStore->reset();
	if(_fStore!=NULL) _fStore->reset();
	if(_pwrStore!=NULL) _pwrStore->reset();
	if(_resultantForcePointsStore!=NULL) _resultantForcePointsStore->reset();
	if(_totFStore!=NULL) _totFStore->reset();
	if(_totTorqueStore!=NULL) _totTorqueStore->reset();

}


//=============================================================================
// IO
//=============================================================================
//_____________________________________________________________________________
/**
 * Print results.
 * 
 * The file names are constructed as
 * aDir + "/" + aBaseName + "_" + ComponentName + aExtension
 *
 * @param aDir Directory in which the results reside.
 * @param aBaseName Base file name.
 * @param aExtension File extension.
 *
 * @return 0 on success, -1 on error.
 */
int Contact::
printResults(const char *aBaseName,const char *aDir,double aDT,
				 const char *aExtension)
{
	if(!getOn()) {
		printf("Contact.printResults: Off- not printing.\n");
		return(-1);
	}
	if(aBaseName==NULL) return(-1);

	// CONSTRUCT PATH
	char path[2048],name[2048];
	if(aDir==NULL) {
		strcpy(path,".");
	} else {
		strcpy(path,aDir);
	}

	// POINTS
	if(aExtension==NULL) {
		sprintf(name,"%s/%s_%s_points",path,aBaseName,getName().c_str());
	} else {
		sprintf(name,"%s/%s_%s_points%s",path,aBaseName,getName().c_str(),
		aExtension);
	}
	if(aDT<=0.0) {
		if(_pStore!=NULL)  _pStore->print(name);
	} else {
		if(_pStore!=NULL)  _pStore->print(name,aDT);
	}

	// VELOCITIES
	if(aExtension==NULL) {
		sprintf(name,"%s/%s_%s_velocities",path,aBaseName,getName().c_str());
	} else {
		sprintf(name,"%s/%s_%s_velocities%s",path,aBaseName,getName().c_str(),
		aExtension);
	}
	if(aDT<=0.0) {
		if(_vStore!=NULL)  _vStore->print(name);
	} else {
		if(_vStore!=NULL)  _vStore->print(name,aDT);
	}

	// FORCES
	if(aExtension==NULL) {
		sprintf(name,"%s/%s_%s_forces",path,aBaseName,getName().c_str());
	} else {
		sprintf(name,"%s/%s_%s_forces%s",path,aBaseName,getName().c_str(),aExtension);
	}
	if(aDT<=0.0) {
		if(_fStore!=NULL)  _fStore->print(name);
	} else {
		if(_fStore!=NULL)  _fStore->print(name,aDT);
	}

	// POWERS
	if(aExtension==NULL) {
		sprintf(name,"%s/%s_%s_powers",path,aBaseName,getName().c_str());
	} else {
		sprintf(name,"%s/%s_%s_powers%s",path,aBaseName,getName().c_str(),aExtension);
	}
	if(aDT<=0.0) {
		if(_pwrStore!=NULL)  _pwrStore->print(name);
	} else {
		if(_pwrStore!=NULL)  _pwrStore->print(name,aDT);
	}

	// RESULTANT FORCE POINT
	if(aExtension==NULL) {
		sprintf(name,"%s/%s_%sresultantForcePoint",path,aBaseName,getName().c_str());
	} else {
		sprintf(name,"%s/%s_%s_resultantForcePoints%s",path,aBaseName,getName().c_str(),aExtension);
	}
	if(aDT<=0.0) {
		if(_resultantForcePointsStore!=NULL)  _resultantForcePointsStore->print(name);
	} else {
		if(_resultantForcePointsStore!=NULL)  _resultantForcePointsStore->print(name,aDT);
	}

	// TOTAL CONTACT FORCES
	if(aExtension==NULL) {
		sprintf(name,"%s/%s_%stotForce",path,aBaseName,getName().c_str());
	} else {
		sprintf(name,"%s/%s_%s_totForce%s",path,aBaseName,getName().c_str(),aExtension);
	}
	if(aDT<=0.0) {
		if(_totFStore!=NULL)  _totFStore->print(name);
	} else {
		if(_totFStore!=NULL)  _totFStore->print(name,aDT);
	}

	// TOTAL CONTACT TORQUES
	if(aExtension==NULL) {
		sprintf(name,"%s/%s_%stotTorque",path,aBaseName,getName().c_str());
	} else {
		sprintf(name,"%s/%s_%s_totTorque%s",path,aBaseName,getName().c_str(),aExtension);
	}
	if(aDT<=0.0) {
		if(_totTorqueStore!=NULL)  _totTorqueStore->print(name);
	} else {
		if(_totTorqueStore!=NULL)  _totTorqueStore->print(name,aDT);
	}

	return(0);
}


