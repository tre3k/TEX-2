/*----- PROTECTED REGION ID(RA153_28.cpp) ENABLED START -----*/
//=============================================================================
//
// file :        RA153_28.cpp
//
// description : C++ source for the RA153_28 class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               RA153_28 are implemented in this file.
//
// project :     
//
// This file is part of Tango device class.
// 
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
// 
//
// Kirill Pshenichnyi (c) 2019
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#include <RA153_28.h>
#include <RA153_28Class.h>

/*----- PROTECTED REGION END -----*/	//	RA153_28.cpp

/**
 *  RA153_28 class description:
 *    
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name  |  Method name
//================================================================
//  State         |  Inherited (no method)
//  Status        |  Inherited (no method)
//  Stop          |  stop
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  rPosition      |  Tango::DevDouble	Scalar
//  aPosition      |  Tango::DevDouble	Scalar
//  rZeroPosition  |  Tango::DevDouble	Scalar
//  LeftEnd        |  Tango::DevBoolean	Scalar
//  CentralEnd     |  Tango::DevBoolean	Scalar
//  RightEnd       |  Tango::DevBoolean	Scalar
//================================================================

namespace RA153_28_ns
{
/*----- PROTECTED REGION ID(RA153_28::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	RA153_28::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : RA153_28::RA153_28()
 *	Description : Constructors for a Tango device
 *                implementing the classRA153_28
 */
//--------------------------------------------------------
RA153_28::RA153_28(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(RA153_28::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::constructor_1
}
//--------------------------------------------------------
RA153_28::RA153_28(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(RA153_28::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::constructor_2
}
//--------------------------------------------------------
RA153_28::RA153_28(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(RA153_28::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : RA153_28::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void RA153_28::delete_device()
{
	DEBUG_STREAM << "RA153_28::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(RA153_28::delete_device) ENABLED START -----*/
	
	//	Delete device allocated objects
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::delete_device
	delete[] attr_rPosition_read;
	delete[] attr_aPosition_read;
	delete[] attr_rZeroPosition_read;
	delete[] attr_LeftEnd_read;
	delete[] attr_CentralEnd_read;
	delete[] attr_RightEnd_read;
}

//--------------------------------------------------------
/**
 *	Method      : RA153_28::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void RA153_28::init_device()
{
	DEBUG_STREAM << "RA153_28::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(RA153_28::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::init_device_before
	

	//	Get the device properties from database
	get_device_property();
	
	attr_rPosition_read = new Tango::DevDouble[1];
	attr_aPosition_read = new Tango::DevDouble[1];
	attr_rZeroPosition_read = new Tango::DevDouble[1];
	attr_LeftEnd_read = new Tango::DevBoolean[1];
	attr_CentralEnd_read = new Tango::DevBoolean[1];
	attr_RightEnd_read = new Tango::DevBoolean[1];
	/*----- PROTECTED REGION ID(RA153_28::init_device) ENABLED START -----*/
	
	if(p9030 != NULL) delete p9030;
	p9030 = new PLX9030::plx9030(device);
	if(p9030->getStatus()!=PLX9030::STATUS_OK){
		device_state = Tango::FAULT;
		device_status = "Error open device " + device + "\n";
		return;
	}

	if(c_ra153_28 != NULL) delete c_ra153_28;
	c_ra153_28 = new cRA153_28::controller_RA153_28(p9030);
	c_ra153_28->encoder_bits = 26;
	c_ra153_28->setChannel(channel);
	c_ra153_28->setSSISpeed(50);
	c_ra153_28->setSpeed(speed);
	c_ra153_28->initMotion();
	c_ra153_28->stopMotion();

	device_state = Tango::ON;
	device_status = Tango::STOP;


	/* DEBUG */
	std::cout << "\t\t\t\tCS0: \t\t\t\tCS1:\n";
	for(int i=0;i<32;i++){
		std::cout << std::dec << i << " - " << std::hex << "0x" << i << ":\t\t0x" << (uint16_t)(p9030->read8(PLX9030::CS0,i)&0xff);
		std::cout << "\t\t\t\t0x" << (uint16_t)(p9030->read8(PLX9030::CS1,i)&0xff) << "\n";
	}
	std::cout << "\n";

	/*----- PROTECTED REGION END -----*/	//	RA153_28::init_device
}

//--------------------------------------------------------
/**
 *	Method      : RA153_28::get_device_property()
 *	Description : Read database to initialize property data members.
 */
//--------------------------------------------------------
void RA153_28::get_device_property()
{
	/*----- PROTECTED REGION ID(RA153_28::get_device_property_before) ENABLED START -----*/
	
	//	Initialize property data members
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::get_device_property_before


	//	Read device properties from database.
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("Device"));
	dev_prop.push_back(Tango::DbDatum("Channel"));
	dev_prop.push_back(Tango::DbDatum("Speed"));
	dev_prop.push_back(Tango::DbDatum("StepToUnit"));
	dev_prop.push_back(Tango::DbDatum("EncoderToUnit"));

	//	is there at least one property to be read ?
	if (dev_prop.size()>0)
	{
		//	Call database and extract values
		if (Tango::Util::instance()->_UseDb==true)
			get_db_device()->get_property(dev_prop);
	
		//	get instance on RA153_28Class to get class property
		Tango::DbDatum	def_prop, cl_prop;
		RA153_28Class	*ds_class =
			(static_cast<RA153_28Class *>(get_device_class()));
		int	i = -1;

		//	Try to initialize Device from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  device;
		else {
			//	Try to initialize Device from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  device;
		}
		//	And try to extract Device value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  device;

		//	Try to initialize Channel from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  channel;
		else {
			//	Try to initialize Channel from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  channel;
		}
		//	And try to extract Channel value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  channel;

		//	Try to initialize Speed from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  speed;
		else {
			//	Try to initialize Speed from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  speed;
		}
		//	And try to extract Speed value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  speed;

		//	Try to initialize StepToUnit from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  stepToUnit;
		else {
			//	Try to initialize StepToUnit from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  stepToUnit;
		}
		//	And try to extract StepToUnit value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  stepToUnit;

		//	Try to initialize EncoderToUnit from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  encoderToUnit;
		else {
			//	Try to initialize EncoderToUnit from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  encoderToUnit;
		}
		//	And try to extract EncoderToUnit value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  encoderToUnit;

	}

	/*----- PROTECTED REGION ID(RA153_28::get_device_property_after) ENABLED START -----*/
	
	//	Check device property data members init
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::get_device_property_after
}

//--------------------------------------------------------
/**
 *	Method      : RA153_28::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void RA153_28::always_executed_hook()
{
	DEBUG_STREAM << "RA153_28::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(RA153_28::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : RA153_28::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void RA153_28::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "RA153_28::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::read_attr_hardware
}
//--------------------------------------------------------
/**
 *	Method      : RA153_28::write_attr_hardware()
 *	Description : Hardware writing for attributes
 */
//--------------------------------------------------------
void RA153_28::write_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "RA153_28::write_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::write_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::write_attr_hardware
}

//--------------------------------------------------------
/**
 *	Read attribute rPosition related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::read_rPosition(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RA153_28::read_rPosition(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::read_rPosition) ENABLED START -----*/
	//	Set the attribute value

	/* DEBUG */

	*attr_rPosition_read = c_ra153_28->getCounts()/stepToUnit;

	if(*attr_rPosition_read == 0.0){
		c_ra153_28->stopMotion();
		device_state = Tango::ON;
		device_status = Tango::STOP;
	}else{
		device_state = Tango::MOVING;
	}

	attr.set_value(attr_rPosition_read);
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::read_rPosition
}
//--------------------------------------------------------
/**
 *	Write attribute rPosition related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::write_rPosition(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "RA153_28::write_rPosition(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevDouble	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(RA153_28::write_rPosition) ENABLED START -----*/

	bool direction;

	if(w_val >= 0) direction = true;
	if(w_val < 0){
		direction = false;
		w_val = -w_val;
	}

	c_ra153_28->runMition(w_val*stepToUnit,direction);

	/*----- PROTECTED REGION END -----*/	//	RA153_28::write_rPosition
}
//--------------------------------------------------------
/**
 *	Read attribute aPosition related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::read_aPosition(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RA153_28::read_aPosition(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::read_aPosition) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_aPosition_read);
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::read_aPosition
}
//--------------------------------------------------------
/**
 *	Write attribute aPosition related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::write_aPosition(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "RA153_28::write_aPosition(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevDouble	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(RA153_28::write_aPosition) ENABLED START -----*/


	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::write_aPosition
}
//--------------------------------------------------------
/**
 *	Read attribute rZeroPosition related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::read_rZeroPosition(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RA153_28::read_rZeroPosition(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::read_rZeroPosition) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_rZeroPosition_read);
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::read_rZeroPosition
}
//--------------------------------------------------------
/**
 *	Write attribute rZeroPosition related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::write_rZeroPosition(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "RA153_28::write_rZeroPosition(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevDouble	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(RA153_28::write_rZeroPosition) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::write_rZeroPosition
}
//--------------------------------------------------------
/**
 *	Read attribute LeftEnd related method
 *	Description: 
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::read_LeftEnd(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RA153_28::read_LeftEnd(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::read_LeftEnd) ENABLED START -----*/
	//	Set the attribute value

	*attr_LeftEnd_read = c_ra153_28->getLeftEnd();
	attr.set_value(attr_LeftEnd_read);
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::read_LeftEnd
}
//--------------------------------------------------------
/**
 *	Read attribute CentralEnd related method
 *	Description: 
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::read_CentralEnd(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RA153_28::read_CentralEnd(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::read_CentralEnd) ENABLED START -----*/
	//	Set the attribute value

	*attr_CentralEnd_read = c_ra153_28->getCentralEnd();
	attr.set_value(attr_CentralEnd_read);

	/*----- PROTECTED REGION END -----*/	//	RA153_28::read_CentralEnd
}
//--------------------------------------------------------
/**
 *	Read attribute RightEnd related method
 *	Description: 
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RA153_28::read_RightEnd(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RA153_28::read_RightEnd(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RA153_28::read_RightEnd) ENABLED START -----*/
	//	Set the attribute value

	*attr_RightEnd_read = c_ra153_28->getRightEnd();
	attr.set_value(attr_RightEnd_read);
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::read_RightEnd
}

//--------------------------------------------------------
/**
 *	Method      : RA153_28::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void RA153_28::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(RA153_28::add_dynamic_attributes) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic attributes if any
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command Stop related method
 *	Description: 
 *
 */
//--------------------------------------------------------
void RA153_28::stop()
{
	DEBUG_STREAM << "RA153_28::Stop()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(RA153_28::stop) ENABLED START -----*/
	
	//	Add your own code
	c_ra153_28->stopMotion();
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::stop
}
//--------------------------------------------------------
/**
 *	Method      : RA153_28::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void RA153_28::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(RA153_28::add_dynamic_commands) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic commands if any
	
	/*----- PROTECTED REGION END -----*/	//	RA153_28::add_dynamic_commands
}

/*----- PROTECTED REGION ID(RA153_28::namespace_ending) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	RA153_28::namespace_ending
} //	namespace
