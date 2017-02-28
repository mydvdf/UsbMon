#include "UsbUtil.h"

extern POBJECT_TYPE *IoDriverObjectType;

extern NTSTATUS ObReferenceObjectByName(
	PUNICODE_STRING ObjectName,
	ULONG Attributes,
	PACCESS_STATE AccessState,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE ObjectType,
	KPROCESSOR_MODE AccessMode,
	PVOID ParseContext,
	PVOID *Object
);
//----------------------------------------------------------------------------------------//
WCHAR* GetUsbHubDriverNameByVersion(USB_HUB_VERSION usb_hub_version)
{
	WCHAR* ret = L"";
	switch (usb_hub_version)
	{
	case USB:
		ret = L"\\Driver\\usbhub";
		break;
	case USB2:
		ret = L"\\Driver\\usbhub";
		break;
	case USB3:
		ret = L"\\Driver\\iusb3hub";
		break;
	default:
		break;
	}
	return ret;
}

//----------------------------------------------------------------------------------------//
NTSTATUS GetUsbHub(USB_HUB_VERSION usb_hub_version, PDRIVER_OBJECT* pDriverObj)
{
	// use such API  
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING DriverName = { 0 };

	WCHAR* Usbhub = GetUsbHubDriverNameByVersion(usb_hub_version);

	RtlInitUnicodeString(&DriverName, Usbhub);

	status = ObReferenceObjectByName(
		&DriverName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
		NULL,
		0,
		*IoDriverObjectType,
		KernelMode,
		NULL,
		(PVOID*)pDriverObj);

	return status;
}


void DumpHidExtension(HID_DEVICE_EXTENSION* hid_common_extension, HID_USB_DEVICE_EXTENSION* mini_extension)
{
 
	STACK_TRACE_DEBUG_INFO(" --->PhysicalDeviceObject: %I64X \r\n", hid_common_extension->PhysicalDeviceObject);
	STACK_TRACE_DEBUG_INFO(" --->DriverObject: %I64X \r\n", hid_common_extension->PhysicalDeviceObject->DriverObject);
	STACK_TRACE_DEBUG_INFO(" --->NextDeviceObject: %I64X \r\n", hid_common_extension->NextDeviceObject);

	STACK_TRACE_DEBUG_INFO("  --->Status: %I64X \r\n", mini_extension->status);
	STACK_TRACE_DEBUG_INFO("  --->DeviceDesc: %I64X \r\n", mini_extension->DeviceDesc);
	STACK_TRACE_DEBUG_INFO("		--->bDeviceClass: %I64X \r\n", mini_extension->DeviceDesc->bDeviceClass);
	STACK_TRACE_DEBUG_INFO("		--->bDeviceSubClass: %I64X \r\n", mini_extension->DeviceDesc->bDeviceSubClass);
	STACK_TRACE_DEBUG_INFO("		--->bDeviceProtocol: %I64X \r\n", mini_extension->DeviceDesc->bDeviceProtocol);
	STACK_TRACE_DEBUG_INFO("		--->bDescriptorType: %I64X \r\n", mini_extension->DeviceDesc->bDescriptorType);
	STACK_TRACE_DEBUG_INFO("		--->bLength: %I64X \r\n", mini_extension->DeviceDesc->bLength);
	STACK_TRACE_DEBUG_INFO("		--->bMaxPacketSize0: %I64X \r\n", mini_extension->DeviceDesc->bMaxPacketSize0);
	STACK_TRACE_DEBUG_INFO("		--->bcdUSB: %I64X \r\n", mini_extension->DeviceDesc->bcdUSB);
	STACK_TRACE_DEBUG_INFO("		--->idProduct: %I64X \r\n", mini_extension->DeviceDesc->idProduct);
	STACK_TRACE_DEBUG_INFO("		--->idVendor: %I64X \r\n", mini_extension->DeviceDesc->idVendor);
	STACK_TRACE_DEBUG_INFO("  --->InterfaceDescDesc: %I64X \r\n", mini_extension->InterfaceDesc);
	STACK_TRACE_DEBUG_INFO("		--->Class: %I64X \r\n", mini_extension->InterfaceDesc->Class);
	STACK_TRACE_DEBUG_INFO("		--->SubClass: %I64X \r\n", mini_extension->InterfaceDesc->SubClass);
	STACK_TRACE_DEBUG_INFO("		--->Protocol: %I64X \r\n", mini_extension->InterfaceDesc->Protocol);
	STACK_TRACE_DEBUG_INFO("		--->NumOfPipes: %I64X \r\n", mini_extension->InterfaceDesc->NumberOfPipes);
	STACK_TRACE_DEBUG_INFO("		--->Length: %I64X \r\n", mini_extension->InterfaceDesc->Length);
	STACK_TRACE_DEBUG_INFO("		--->AlternateSetting: %I64X \r\n", mini_extension->InterfaceDesc->AlternateSetting);

	/*
	HID_DESCRIPTOR* hid_desc = (HID_DESCRIPTOR*)(mini_extension + 0x57);

	STACK_TRACE_DEBUG_INFO("  --->DescList : %I64X \r\n", FIELD_OFFSET(HID_USB_DEVICE_EXTENSION, descriptorList));
	STACK_TRACE_DEBUG_INFO("  --->DescList : %I64X %I64X \r\n", hid_desc, mini_extension->descriptorList);
	STACK_TRACE_DEBUG_INFO("	--->bcdHID: %I64X \r\n", hid_desc->bcdHID);
	STACK_TRACE_DEBUG_INFO("	--->bCountry: %I64X \r\n", hid_desc->bCountry);
	STACK_TRACE_DEBUG_INFO("	--->bDescriptorType: %I64X \r\n", hid_desc->bDescriptorType);
	STACK_TRACE_DEBUG_INFO("	--->bLength: %I64X \r\n", hid_desc->bLength);
	STACK_TRACE_DEBUG_INFO("	--->bNumDescriptors: %I64X \r\n", hid_desc->bNumDescriptors);
	*/
}


VOID	  DumpHidMiniDriverExtension(
	HID_DEVICE_EXTENSION* hid_common_extension
)
{
	if (!hid_common_extension)
	{
		return;
	}

	if (!hid_common_extension->MiniDeviceExtension)
	{
		return;
	}

	DumpHidExtension(hid_common_extension, hid_common_extension->MiniDeviceExtension);
}

VOID DumpUrb(PURB urb)
{
	struct _URB_HEADER urb_header = urb->UrbHeader;;

	STACK_TRACE_DEBUG_INFO("---------------URB HEADER------------- \r\n");
	STACK_TRACE_DEBUG_INFO("- Length: %x \r\n", urb_header.Length);
	STACK_TRACE_DEBUG_INFO("- Function: %x \r\n", urb_header.Function);
	STACK_TRACE_DEBUG_INFO("- Status: %x \r\n", urb_header.Status);
	STACK_TRACE_DEBUG_INFO("- UsbdDeviceHandle: %x \r\n", urb_header.UsbdDeviceHandle);
	STACK_TRACE_DEBUG_INFO("- UsbdFlags: %x \r\n", urb_header.UsbdFlags);
	STACK_TRACE_DEBUG_INFO("---------------URB Function------------- \r\n");

	switch (urb_header.Function)
	{
#if (NTDDI_VERSION >= NTDDI_WIN8)
	case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER_USING_CHAINED_MDL:
		break;
	case URB_FUNCTION_OPEN_STATIC_STREAMS:
		break;
	case URB_FUNCTION_CLOSE_STATIC_STREAMS:
		break;
#endif
	case URB_FUNCTION_SELECT_CONFIGURATION:
		break;
	case URB_FUNCTION_SELECT_INTERFACE:
		break;
	case URB_FUNCTION_ABORT_PIPE:
		break;
	case URB_FUNCTION_TAKE_FRAME_LENGTH_CONTROL:
		break;
	case URB_FUNCTION_RELEASE_FRAME_LENGTH_CONTROL:
		break;
	case URB_FUNCTION_GET_FRAME_LENGTH:
		break;
	case URB_FUNCTION_SET_FRAME_LENGTH:
		break;
	case URB_FUNCTION_GET_CURRENT_FRAME_NUMBER:
		break;
	case URB_FUNCTION_CONTROL_TRANSFER:
		break;
	case URB_FUNCTION_CONTROL_TRANSFER_EX:
		break;
	case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER:
	{
		//			struct _URB_BULK_OR_INTERRUPT_TRANSFER* data  = (struct _URB_BULK_OR_INTERRUPT_TRANSFER*) &urb->UrbHeader;

		//			STACK_TRACE_COMMON_DBG_BREAK("_URB_BULK_OR_INTERRUPT_TRANSFER \r\n"); 
		/*			if (data->TransferFlags & USBD_TRANSFER_DIRECTION_IN)
		{
		STACK_TRACE_COMMON_DBG_BREAK("USBD_TRANSFER_DIRECTION_IN \r\n");
		}
		if (data->TransferFlags & USBD_TRANSFER_DIRECTION_OUT)
		{
		STACK_TRACE_COMMON_DBG_BREAK("USBD_TRANSFER_DIRECTION_OUT \r\n");

		}
		if (data->TransferFlags & USBD_SHORT_TRANSFER_OK)
		{
		STACK_TRACE_COMMON_DBG_BREAK("USBD_SHORT_TRANSFER_OK \r\n");
		}*/

	}
	break;
	case URB_FUNCTION_RESET_PIPE:
		break;
	case URB_FUNCTION_SYNC_RESET_PIPE:
		break;
	case URB_FUNCTION_SYNC_CLEAR_STALL:
		break;
	case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE:
		break;
	case URB_FUNCTION_GET_DESCRIPTOR_FROM_ENDPOINT:
		break;
	case URB_FUNCTION_SET_DESCRIPTOR_TO_DEVICE:
		break;
	case URB_FUNCTION_SET_DESCRIPTOR_TO_ENDPOINT:
		break;
	case URB_FUNCTION_SET_FEATURE_TO_DEVICE:
		break;
	case URB_FUNCTION_SET_FEATURE_TO_INTERFACE:
		break;
	case URB_FUNCTION_SET_FEATURE_TO_ENDPOINT:
		break;
	case URB_FUNCTION_SET_FEATURE_TO_OTHER:
		break;
	case URB_FUNCTION_CLEAR_FEATURE_TO_DEVICE:
		break;
	case URB_FUNCTION_CLEAR_FEATURE_TO_INTERFACE:
		break;
	case URB_FUNCTION_CLEAR_FEATURE_TO_ENDPOINT:
		break;
	case URB_FUNCTION_CLEAR_FEATURE_TO_OTHER:
		break;
	case URB_FUNCTION_GET_STATUS_FROM_DEVICE:
		break;
	case URB_FUNCTION_GET_STATUS_FROM_INTERFACE:
		break;
	case URB_FUNCTION_GET_STATUS_FROM_ENDPOINT:
		break;
	case URB_FUNCTION_GET_STATUS_FROM_OTHER:
		break;
	case URB_FUNCTION_VENDOR_DEVICE:
		break;
	case URB_FUNCTION_VENDOR_INTERFACE:
		break;
	case URB_FUNCTION_VENDOR_ENDPOINT:
		break;
	case URB_FUNCTION_VENDOR_OTHER:
		break;
	case URB_FUNCTION_CLASS_DEVICE:
		break;
	case URB_FUNCTION_CLASS_INTERFACE:
		break;
	case URB_FUNCTION_CLASS_ENDPOINT:
		break;
	case URB_FUNCTION_CLASS_OTHER:
		break;
	case URB_FUNCTION_GET_CONFIGURATION:
		break;
	case URB_FUNCTION_GET_INTERFACE:
		break;
	case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE:
		break;
	case URB_FUNCTION_SET_DESCRIPTOR_TO_INTERFACE:
		break;
	case URB_FUNCTION_GET_MS_FEATURE_DESCRIPTOR:
		break;

	}
}