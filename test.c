// use -ldns_sd to compile this code


#include<stdio.h>
#include<dns_sd.h>
#include<inttypes.h>
#include<string.h>

#define DNSServiceFlags 0
#define InterfaceIndex 0
#define ServiceName "_hap._tcp"
#define ServiceType ""
#define port 0
#define deviceIdentity "00:11:22:33:44:55"
#define deviceName "Light"
int main()
{
	TXTRecordRef txtRecord;
	DNSServiceRef sdref;
	DNSServiceErrorType err;
	
	uint32_t ccn = 1;
	char ccn_buff[11];

	uint32_t stn = 1;
	char stn_buff[11];

	int numbytes1,numbytes2;

	TXTRecordCreate(&txtRecord, 0, NULL);

	numbytes1 = sprintf(ccn_buff, "%"PRIu32"", ccn);
	TXTRecordSetValue(&txtRecord, "c#", numbytes1, ccn_buff);    //Current Config number
	TXTRecordSetValue(&txtRecord, "ff", 4, "0x01");    //feature flag, 0x01 Supports MFi-pair
	TXTRecordSetValue(&txtRecord, "id", 17, deviceIdentity);    //Device id
	TXTRecordSetValue(&txtRecord, "md", strlen(deviceName), deviceName);    //Model Name		
//	TXTRecordSetValue(&txtRecord, "pv", 3, "1.0");  //Version, required if not 1.0
	numbytes2 = sprintf(stn_buff, "%"PRIu32"", stn);
    	TXTRecordSetValue(&txtRecord, "s#", numbytes2, stn_buff);    //Current Status number
//    	TXTRecordSetValue(&txtRecord, "sf", 1, "1");    //Status Flags, required if non zero




	err = DNSServiceRegister(&sdref,DNSServiceFlags,InterfaceIndex,ServiceName,ServiceType,NULL,NULL,port,TXTRecordGetLength(&txtRecord),TXTRecordGetBytesPtr(&txtRecord),NULL,NULL);
	if(err != kDNSServiceErr_NoError) {
		printf("error registering service\n");
		return -1;
	}	
	TXTRecordDeallocate(&txtRecord);
	



	return 0;
}
