/*----- PROTECTED REGION ID(Pneumo.cpp) ENABLED START -----*/
//=============================================================================
//
// file :        Pneumo.cpp
//
// description : C++ source for the Pneumo class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               Pneumo are implemented in this file.
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
// Pshenichnyi Kirill PNPI (c) 2019
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#include <Pneumo.h>
#include <PneumoClass.h>

/*----- PROTECTED REGION END -----*/	//	Pneumo.cpp

/**
 *  Pneumo class description:
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
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  valve      |  Tango::DevBoolean	Scalar
//  sensor     |  Tango::DevBoolean	Scalar
//  allsensor  |  Tango::DevUShort	Scalar
//================================================================

namespace Pneumo_ns
{
/*----- PROTECTED REGION ID(Pneumo::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	Pneumo::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : Pneumo::Pneumo()
 *	Description : Constructors for a Tango device
 *                implementing the classPneumo
 */
//--------------------------------------------------------
Pneumo::Pneumo(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(Pneumo::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::constructor_1
}
//--------------------------------------------------------
Pneumo::Pneumo(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(Pneumo::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::constructor_2
}
//--------------------------------------------------------
Pneumo::Pneumo(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(Pneumo::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : Pneumo::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void Pneumo::delete_device()
{
	DEBUG_STREAM << "Pneumo::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(Pneumo::delete_device) ENABLED START -----*/
	
	//	Delete device allocated objects
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::delete_device
	delete[] attr_sensor_read;
	delete[] attr_allsensor_read;
}

//--------------------------------------------------------
/**
 *	Method      : Pneumo::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void Pneumo::init_device()
{
	DEBUG_STREAM << "Pneumo::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(Pneumo::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::init_device_before
	

	//	Get the device properties from database
	get_device_property();
	
	attr_sensor_read = new Tango::DevBoolean[1];
	attr_allsensor_read = new Tango::DevUShort[1];
	/*----- PROTECTED REGION ID(Pneumo::init_device) ENABLED START -----*/

	sp = static_cast<PneumoClass *>(get_device_class())->sp;
	if(sp==NULL){
		static_cast<PneumoClass *>(get_device_class())->sp = new SP::SerialPort(serialPort.c_str());
		sp = static_cast<PneumoClass *>(get_device_class())->sp;
	}
	if(pneumo==NULL) pneumo = new Pneumatics(sp,deviceAddr);


	device_state = Tango::OFF;
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::init_device
}

//--------------------------------------------------------
/**
 *	Method      : Pneumo::get_device_property()
 *	Description : Read database to initialize property data members.
 */
//--------------------------------------------------------
void Pneumo::get_device_property()
{
	/*----- PROTECTED REGION ID(Pneumo::get_device_property_before) ENABLED START -----*/
	
	//	Initialize property data members
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::get_device_property_before


	//	Read device properties from database.
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("SerialPort"));
	dev_prop.push_back(Tango::DbDatum("DeviceAddr"));
	dev_prop.push_back(Tango::DbDatum("NumOfValve"));

	//	is there at least one property to be read ?
	if (dev_prop.size()>0)
	{
		//	Call database and extract values
		if (Tango::Util::instance()->_UseDb==true)
			get_db_device()->get_property(dev_prop);
	
		//	get instance on PneumoClass to get class property
		Tango::DbDatum	def_prop, cl_prop;
		PneumoClass	*ds_class =
			(static_cast<PneumoClass *>(get_device_class()));
		int	i = -1;

		//	Try to initialize SerialPort from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  serialPort;
		else {
			//	Try to initialize SerialPort from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  serialPort;
		}
		//	And try to extract SerialPort value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  serialPort;

		//	Try to initialize DeviceAddr from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  deviceAddr;
		else {
			//	Try to initialize DeviceAddr from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  deviceAddr;
		}
		//	And try to extract DeviceAddr value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  deviceAddr;

		//	Try to initialize NumOfValve from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  numOfValve;
		else {
			//	Try to initialize NumOfValve from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  numOfValve;
		}
		//	And try to extract NumOfValve value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  numOfValve;

	}

	/*----- PROTECTED REGION ID(Pneumo::get_device_property_after) ENABLED START -----*/
	
	//	Check device property data members init
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::get_device_property_after
}

//--------------------------------------------------------
/**
 *	Method      : Pneumo::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void Pneumo::always_executed_hook()
{
	DEBUG_STREAM << "Pneumo::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(Pneumo::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : Pneumo::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void Pneumo::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "Pneumo::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(Pneumo::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::read_attr_hardware
}
//--------------------------------------------------------
/**
 *	Method      : Pneumo::write_attr_hardware()
 *	Description : Hardware writing for attributes
 */
//--------------------------------------------------------
void Pneumo::write_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "Pneumo::write_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(Pneumo::write_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::write_attr_hardware
}

//--------------------------------------------------------
/**
 *	Write attribute valve related method
 *	Description: state of valve
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Pneumo::write_valve(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "Pneumo::write_valve(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevBoolean	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(Pneumo::write_valve) ENABLED START -----*/

	/* if sensor is false */
	if(!(*attr_sensor_read)){
		device_state = Tango::FAULT;
		device_status = "No pressure in system!";
		//return;
	}

	if(w_val){
		(static_cast<PneumoClass *>(get_device_class()))->reg_in |= (1 << numOfValve);
		pneumo->setRegister((static_cast<PneumoClass *>(get_device_class()))->reg_in);
		device_status = "in air";
		device_state = Tango::ON;
	}else{
		(static_cast<PneumoClass *>(get_device_class()))->reg_in &= ~(1 << numOfValve);
		pneumo->setRegister((static_cast<PneumoClass *>(get_device_class()))->reg_in);
		device_status = "is down";
		device_state = Tango::OFF;
	}

	/*----- PROTECTED REGION END -----*/	//	Pneumo::write_valve
}
//--------------------------------------------------------
/**
 *	Read attribute sensor related method
 *	Description: Sensor of pressure
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Pneumo::read_sensor(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pneumo::read_sensor(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(Pneumo::read_sensor) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_sensor_read);

	if((pneumo->getRegister() & (1 << numOfValve))){
		*attr_sensor_read = false;
		device_state = Tango::FAULT;
		device_status = "No pressure in system!";
	}else{
        device_status = "Pressure in system.";
		*attr_sensor_read = true;
	}

    //std::cout << std::hex << pneumo->getRegister() << "\n";
	if((pneumo->getRegister() & (1 << numOfValve))){
	    std::cout << numOfValve << " sensor not active (OFF)\n";
	}else{
        std::cout << numOfValve << " sensor is active (ON)\n";
	}

	/*----- PROTECTED REGION END -----*/	//	Pneumo::read_sensor
}
//--------------------------------------------------------
/**
 *	Read attribute allsensor related method
 *	Description: 
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Pneumo::read_allsensor(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pneumo::read_allsensor(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(Pneumo::read_allsensor) ENABLED START -----*/

	*attr_allsensor_read = pneumo->getRegister();

	//	Set the attribute value
	attr.set_value(attr_allsensor_read);
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::read_allsensor
}

//--------------------------------------------------------
/**
 *	Method      : Pneumo::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void Pneumo::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(Pneumo::add_dynamic_attributes) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic attributes if any
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : Pneumo::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void Pneumo::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(Pneumo::add_dynamic_commands) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic commands if any
	
	/*----- PROTECTED REGION END -----*/	//	Pneumo::add_dynamic_commands
}

/*----- PROTECTED REGION ID(Pneumo::namespace_ending) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	Pneumo::namespace_ending
} //	namespace
