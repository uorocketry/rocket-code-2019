/*!
 *	\file		ellipseMinimal.c
 *  \author		SBG Systems (Alexis GUINAMARD)
 *	\date		28/03/2014
 *
 *	\brief		C example that simply opens an Ellipse interface and reads Euler Angles from it.
 *
 *	This small example demonstrates how to initialize the sbgECom library
 *	to read data from an Ellipse using callbacks.
 *
 *	\section CodeCopyright Copyright Notice 
 *	Copyright (C) 2007-2014, SBG Systems SAS. All rights reserved.
 *	
 *	This source code is intended for use only by SBG Systems SAS and
 *	those that have explicit written permission to use it from
 *	SBG Systems SAS.
 *	
 *	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 *	PARTICULAR PURPOSE.
 */
#include <sbgEComLib.h>
#include <time.h>

//----------------------------------------------------------------------//
//  Call backs                                                          //
//----------------------------------------------------------------------//

/*!
 *	Callback definition called each time a new log is received.
 *	\param[in]	pHandle									Valid handle on the sbgECom instance that has called this callback.
 *	\param[in]	msgClass								Class of the message we have received
 *	\param[in]	msg										Message ID of the log received.
 *	\param[in]	pLogData								Contains the received log data as an union.
 *	\param[in]	pUserArg								Optional user supplied argument.
 *	\return												SBG_NO_ERROR if the received log has been used successfully.
 */
SbgErrorCode onLogReceived(SbgEComHandle *pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData *pLogData, void *pUserArg)
{
	//
	// Handle separately each received data according to the log ID
	//
	switch (msg)
	{
	case SBG_ECOM_LOG_EKF_EULER:
		//
		// Simply display euler angles in real time
		//
		printf("Euler Angles: %3.1f\t%3.1f\t%3.1f\tStd Dev:%3.1f\t%3.1f\t%3.1f   \r", 
				sbgRadToDegF(pLogData->ekfEulerData.euler[0]), sbgRadToDegF(pLogData->ekfEulerData.euler[1]), sbgRadToDegF(pLogData->ekfEulerData.euler[2]), 
				sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[0]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[1]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[2]));
		break;
	default:
		break;
	}
	
	return SBG_NO_ERROR;
}

//----------------------------------------------------------------------//
//  Main program                                                        //
//----------------------------------------------------------------------//

/*!
 *	Main entry point.
 *	\param[in]	argc		Number of input arguments.
 *	\param[in]	argv		Input arguments as an array of strings.
 *	\return					0 if no error and -1 in case of error.
 */
int main(int argc, char** argv)
{
	SbgEComHandle			comHandle;
	SbgErrorCode			errorCode;
	SbgInterface			sbgInterface;
	int32					retValue = 0;
	SbgEComDeviceInfo		deviceInfo;

	//
	// Create an interface: 
	// We can choose either a serial for real time operation, or file for previously logged data parsing
	// Note interface closing is also differentiated !
	//
	errorCode = sbgInterfaceSerialCreate(&sbgInterface, "/dev/cu.usbserial-FTX2GE57", 921600);		// Example for Unix using a FTDI Usb2Uart converter
	//errorCode = sbgInterfaceSerialCreate(&sbgInterface, "COM3", 115200);							// Example for Windows serial communication

	//
	// Test that the interface has been created
	//
	if (errorCode == SBG_NO_ERROR)
	{
		//
		// Create the sbgECom library and associate it with the created interfaces
		//
		errorCode = sbgEComInit(&comHandle, &sbgInterface);

		//
		// Test that the sbgECom has been initialized
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Get device inforamtions
			//
			errorCode = sbgEComCmdGetInfo(&comHandle, &deviceInfo);

			//
			// Display device information if no error
			//
			if (errorCode == SBG_NO_ERROR)
			{
				printf("Device : %0.9u found\n", deviceInfo.serialNumber);
			}
			else
			{
				fprintf(stderr, "ellipseMinimal: Unable to get device information.\n");
			}

			//
			// Configure some output logs to 25 Hz
			//
			if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_IMU_DATA, SBG_ECOM_OUTPUT_MODE_DIV_8) != SBG_NO_ERROR)
			{
				fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_IMU_DATA.\n");
			}
			if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_EULER, SBG_ECOM_OUTPUT_MODE_DIV_8) != SBG_NO_ERROR)
			{
				fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_EKF_EULER.\n");
			}
			
			//
			// Display a message for real time data display
			//
			printf("sbgECom properly Initialized.\n");
			printf("sbgECom version %s\n\n", SBG_E_COM_VERSION_STR);
			printf("Euler Angles display with estimated standard deviation.\n");

			//
			// Define callbacks for received data
			//
			sbgEComSetReceiveLogCallback(&comHandle, onLogReceived, NULL);

			//
			// Loop until the user exist
			//
			while (1)
			{
				//
				// Try to read a frame
				//
				errorCode = sbgEComHandle(&comHandle);

				//
				// Test if we have to release some CPU (no frame received)
				//
				if (errorCode == SBG_NOT_READY)
				{
					//
					// Release CPU
					//
					sbgSleep(1);
				}
				else
				{
					fprintf(stderr, "Error\n");
				}
			}

			//
			// Close the sbgEcom library
			//
			sbgEComClose(&comHandle);
		}
		else
		{
			//
			// Unable to initialize the sbgECom
			//
			fprintf(stderr, "ellipseMinimal: Unable to initialize the sbgECom library.\n");
			retValue = -1;
		}
		
		//
		// Close the interface
		//
		sbgInterfaceSerialDestroy(&sbgInterface);		
	}
	else
	{
		//
		// Unable to create the interface
		//
		fprintf(stderr, "ellipseMinimal: Unable to create the interface.\n");
		retValue = -1;
	}

	//
	// Returns -1 if we have an error
	//
	return retValue;
}
