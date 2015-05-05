/* Homekit Bonjour service for linux machines. (requires avahi daemon)
 * Created by: Mayur Kulkarni
 * V 1.0
 * use gcc <filename> -o <output-filename> -ldns_sd to compile this code 
 */

// 


#include<stdio.h>
#include<dns_sd.h>
#include<inttypes.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>

//device id for bonjour service
#define deviceIdentity "00:11:22:33:44:55"
//Service Name
#define deviceName "Light"


//service will be bound to any port, so this method returns the port number which it has bound to.
unsigned short getSocketPortNumberV4(int _socket) {
	struct sockaddr_in addr; 
	socklen_t len = sizeof(addr);
	getsockname(_socket, (struct sockaddr *)&addr, &len);
	return ntohs(addr.sin_port);
}

int main()
{
	TXTRecordRef txtRecord;
	DNSServiceRef sdref;
	DNSServiceErrorType err;
	
	uint32_t ccn = 1;
	char ccn_buff[11];

	uint32_t stn = 1;
	char stn_buff[11];

	int numbytes1,numbytes2,ret;

	//create Text Records which will be passed to create bonjour service
	TXTRecordCreate(&txtRecord, 0, NULL);

	numbytes1 = sprintf(ccn_buff, "%"PRIu32"", ccn);			//parse current config number to string
	ret = TXTRecordSetValue(&txtRecord, "c#", numbytes1, ccn_buff);    	//Current Config number
	if(ret != kDNSServiceErr_NoError) {	printf("error in TXTRecordSetValue: %d\n",ret); }

	ret = TXTRecordSetValue(&txtRecord, "ff", 4, "0x00");    		//feature flag, 0x01 Supports MFi-pair
	if(ret != kDNSServiceErr_NoError) {  printf("error in TXTRecordSetValue: %d\n",ret); }

	ret = TXTRecordSetValue(&txtRecord, "id", 17, deviceIdentity);    	//Device id
	if(ret != kDNSServiceErr_NoError) {  printf("error in TXTRecordSetValue: %d\n",ret); }
	
	ret = TXTRecordSetValue(&txtRecord, "md", strlen(deviceName), deviceName);    //Model Name		
	if(ret != kDNSServiceErr_NoError) {  printf("error in TXTRecordSetValue: %d\n",ret); }

//	ret = TXTRecordSetValue(&txtRecord, "pv", 3, "1.0");  //Version, required if not 1.0
//	if(ret != kDNSServiceErr_NoError) {  printf("error in TXTRecordSetValue: %d\n",ret); }
	
	numbytes2 = sprintf(stn_buff, "%"PRIu32"", stn);
    	ret = TXTRecordSetValue(&txtRecord, "s#", numbytes2, stn_buff);    //Current Status number
	if(ret != kDNSServiceErr_NoError) {  printf("error in TXTRecordSetValue: %d\n",ret); }

//    	ret = TXTRecordSetValue(&txtRecord, "sf", 1, "1");    //Status Flags, required if non zero
//	if(ret != kDNSServiceErr_NoError) {  printf("error in TXTRecordSetValue: %d\n",ret); }

	//registers Bonjour service (see dns_sd.h for details)
	//service will be created as _hap._tcp.light
	err = DNSServiceRegister(&sdref,0,0,deviceName,"_hap._tcp",NULL,NULL,htons(0),TXTRecordGetLength(&txtRecord),TXTRecordGetBytesPtr(&txtRecord),NULL,NULL);
	if(err != kDNSServiceErr_NoError) {
		printf("error registering service %d\n",err);
		return -1;
	}
	
	//deallocate created text records after registering the service
	TXTRecordDeallocate(&txtRecord);

	while(1);	



	return 0;
}
