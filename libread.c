/***************************************************
 *author:admin@9crk.com
 *禁止拷贝，禁止下载！
 ***************************************************/
#include<string.h>
#include<stdio.h>
static const char headsA[]={0xFF,0xF1};
static const char heads[]={0x00, 0x00, 0x00, 0x01};
static char head[4],headA[2];
extern int readHeader(FILE* tmpFp);
extern int readFrame(FILE* tmpFp, char* pBuffer);

int readHeader(FILE* tmpFp)
{
	fread(head, 4, 1, tmpFp);
	if(strcmp(head, heads) != 0){
		printf("error header!\n");
		return -1;
	}
	return 0;	
}
int readHeaderA(FILE* tmpFp)
{
	fread(headA, 2, 1, tmpFp);
	if(strcmp(headA, headsA) != 0){
		printf("error headerA!\n");
		return -1;
	}
	return 0;	
}
int hexdump(char* ptr, int len)
{
	int i;
	for(i=0;i<len;i++){
		if(i%16 == 0)printf("\n");
		printf("%02x ", (unsigned char)*ptr);
		ptr++;
	}
}
int readFrame(FILE* tmpFp, char* pBuffer)
{
	int ch;
	int index;
	unsigned int count;

	count = 0;
	index = 0;
	memcpy((void *)pBuffer, (void*)heads, 4);
	pBuffer += 4;
	while(!feof(tmpFp)){
		ch = fgetc(tmpFp);
		*pBuffer = ch;
		pBuffer++;
		count ++;
		if(ch == 0x00 || ch == 0x01){
			head[index] = ch;
			index++;
			if(index == 3 && strcmp(head,&heads[1]) == 0){
				//printf("NAL\n");
			}
			if(index == 4 && strcmp(head, heads) == 0){
				if(count > 32)break;
				else index = 0;
			}
		}else{
			index=0;
		}
	}
	return count;
}
int readFrameA(FILE* tmpFp, char* pBuffer)
{
	int ch;
	int index;
	unsigned int count;

	count = 0;
	index = 0;
	memcpy((void *)pBuffer, (void*)headsA, 2);
	pBuffer += 2;
	while(!feof(tmpFp)){
		ch = fgetc(tmpFp);
		*pBuffer = ch;
		pBuffer++;
		count ++;
		if(ch == 0xFF || ch == 0xF1){
			headA[index] = ch;
			index++;
			if(index == 2 && strcmp(headA, headsA) == 0){
				break;
			}
		}else{
			index=0;
		}
	}
	return count;
}




