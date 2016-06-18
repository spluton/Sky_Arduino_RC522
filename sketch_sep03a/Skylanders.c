
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
void(* resetFunc) (void) = 0; //declare reset function @ address 0

extern uint64_t pgm_read_uint64 (const uint64_t*);

__asm__ (
  ".global pgm_read_uint64\n"
  "pgm_read_uint64:\n"
  "    movw r30, r24\n"
  "    lpm  r18, Z+\n"
  "    lpm  r19, Z+\n"
  "    lpm  r20, Z+\n"
  "    lpm  r21, Z+\n"
  "    lpm  r22, Z+\n"
  "    lpm  r23, Z+\n"
  "    lpm  r24, Z+\n"
  "    lpm  r25, Z+\n"
  "    ret\n"
);




extern const __flash uint64_t crc64_table[]  PROGMEM = {
  0x0000000000000000, 0x42F0E1EBA9EA3693, 0x85E1C3D753D46D26, 0xC711223CFA3E5BB5,
  0x493366450E42ECDF, 0x0BC387AEA7A8DA4C, 0xCCD2A5925D9681F9, 0x8E224479F47CB76A,
  0x9266CC8A1C85D9BE, 0xD0962D61B56FEF2D, 0x17870F5D4F51B498, 0x5577EEB6E6BB820B,
  0xDB55AACF12C73561, 0x99A54B24BB2D03F2, 0x5EB4691841135847, 0x1C4488F3E8F96ED4,
  0x663D78FF90E185EF, 0x24CD9914390BB37C, 0xE3DCBB28C335E8C9, 0xA12C5AC36ADFDE5A,
  0x2F0E1EBA9EA36930, 0x6DFEFF5137495FA3, 0xAAEFDD6DCD770416, 0xE81F3C86649D3285,
  0xF45BB4758C645C51, 0xB6AB559E258E6AC2, 0x71BA77A2DFB03177, 0x334A9649765A07E4,
  0xBD68D2308226B08E, 0xFF9833DB2BCC861D, 0x388911E7D1F2DDA8, 0x7A79F00C7818EB3B,
  0xCC7AF1FF21C30BDE, 0x8E8A101488293D4D, 0x499B3228721766F8, 0x0B6BD3C3DBFD506B,
  0x854997BA2F81E701, 0xC7B97651866BD192, 0x00A8546D7C558A27, 0x4258B586D5BFBCB4,
  0x5E1C3D753D46D260, 0x1CECDC9E94ACE4F3, 0xDBFDFEA26E92BF46, 0x990D1F49C77889D5,
  0x172F5B3033043EBF, 0x55DFBADB9AEE082C, 0x92CE98E760D05399, 0xD03E790CC93A650A,
  0xAA478900B1228E31, 0xE8B768EB18C8B8A2, 0x2FA64AD7E2F6E317, 0x6D56AB3C4B1CD584,
  0xE374EF45BF6062EE, 0xA1840EAE168A547D, 0x66952C92ECB40FC8, 0x2465CD79455E395B,
  0x3821458AADA7578F, 0x7AD1A461044D611C, 0xBDC0865DFE733AA9, 0xFF3067B657990C3A,
  0x711223CFA3E5BB50, 0x33E2C2240A0F8DC3, 0xF4F3E018F031D676, 0xB60301F359DBE0E5,
  0xDA050215EA6C212F, 0x98F5E3FE438617BC, 0x5FE4C1C2B9B84C09, 0x1D14202910527A9A,
  0x93366450E42ECDF0, 0xD1C685BB4DC4FB63, 0x16D7A787B7FAA0D6, 0x5427466C1E109645,
  0x4863CE9FF6E9F891, 0x0A932F745F03CE02, 0xCD820D48A53D95B7, 0x8F72ECA30CD7A324,
  0x0150A8DAF8AB144E, 0x43A04931514122DD, 0x84B16B0DAB7F7968, 0xC6418AE602954FFB,
  0xBC387AEA7A8DA4C0, 0xFEC89B01D3679253, 0x39D9B93D2959C9E6, 0x7B2958D680B3FF75,
  0xF50B1CAF74CF481F, 0xB7FBFD44DD257E8C, 0x70EADF78271B2539, 0x321A3E938EF113AA,
  0x2E5EB66066087D7E, 0x6CAE578BCFE24BED, 0xABBF75B735DC1058, 0xE94F945C9C3626CB,
  0x676DD025684A91A1, 0x259D31CEC1A0A732, 0xE28C13F23B9EFC87, 0xA07CF2199274CA14,
  0x167FF3EACBAF2AF1, 0x548F120162451C62, 0x939E303D987B47D7, 0xD16ED1D631917144,
  0x5F4C95AFC5EDC62E, 0x1DBC74446C07F0BD, 0xDAAD56789639AB08, 0x985DB7933FD39D9B,
  0x84193F60D72AF34F, 0xC6E9DE8B7EC0C5DC, 0x01F8FCB784FE9E69, 0x43081D5C2D14A8FA,
  0xCD2A5925D9681F90, 0x8FDAB8CE70822903, 0x48CB9AF28ABC72B6, 0x0A3B7B1923564425,
  0x70428B155B4EAF1E, 0x32B26AFEF2A4998D, 0xF5A348C2089AC238, 0xB753A929A170F4AB,
  0x3971ED50550C43C1, 0x7B810CBBFCE67552, 0xBC902E8706D82EE7, 0xFE60CF6CAF321874,
  0xE224479F47CB76A0, 0xA0D4A674EE214033, 0x67C58448141F1B86, 0x253565A3BDF52D15,
  0xAB1721DA49899A7F, 0xE9E7C031E063ACEC, 0x2EF6E20D1A5DF759, 0x6C0603E6B3B7C1CA,
  0xF6FAE5C07D3274CD, 0xB40A042BD4D8425E, 0x731B26172EE619EB, 0x31EBC7FC870C2F78,
  0xBFC9838573709812, 0xFD39626EDA9AAE81, 0x3A28405220A4F534, 0x78D8A1B9894EC3A7,
  0x649C294A61B7AD73, 0x266CC8A1C85D9BE0, 0xE17DEA9D3263C055, 0xA38D0B769B89F6C6,
  0x2DAF4F0F6FF541AC, 0x6F5FAEE4C61F773F, 0xA84E8CD83C212C8A, 0xEABE6D3395CB1A19,
  0x90C79D3FEDD3F122, 0xD2377CD44439C7B1, 0x15265EE8BE079C04, 0x57D6BF0317EDAA97,
  0xD9F4FB7AE3911DFD, 0x9B041A914A7B2B6E, 0x5C1538ADB04570DB, 0x1EE5D94619AF4648,
  0x02A151B5F156289C, 0x4051B05E58BC1E0F, 0x87409262A28245BA, 0xC5B073890B687329,
  0x4B9237F0FF14C443, 0x0962D61B56FEF2D0, 0xCE73F427ACC0A965, 0x8C8315CC052A9FF6,
  0x3A80143F5CF17F13, 0x7870F5D4F51B4980, 0xBF61D7E80F251235, 0xFD913603A6CF24A6,
  0x73B3727A52B393CC, 0x31439391FB59A55F, 0xF652B1AD0167FEEA, 0xB4A25046A88DC879,
  0xA8E6D8B54074A6AD, 0xEA16395EE99E903E, 0x2D071B6213A0CB8B, 0x6FF7FA89BA4AFD18,
  0xE1D5BEF04E364A72, 0xA3255F1BE7DC7CE1, 0x64347D271DE22754, 0x26C49CCCB40811C7,
  0x5CBD6CC0CC10FAFC, 0x1E4D8D2B65FACC6F, 0xD95CAF179FC497DA, 0x9BAC4EFC362EA149,
  0x158E0A85C2521623, 0x577EEB6E6BB820B0, 0x906FC95291867B05, 0xD29F28B9386C4D96,
  0xCEDBA04AD0952342, 0x8C2B41A1797F15D1, 0x4B3A639D83414E64, 0x09CA82762AAB78F7,
  0x87E8C60FDED7CF9D, 0xC51827E4773DF90E, 0x020905D88D03A2BB, 0x40F9E43324E99428,
  0x2CFFE7D5975E55E2, 0x6E0F063E3EB46371, 0xA91E2402C48A38C4, 0xEBEEC5E96D600E57,
  0x65CC8190991CB93D, 0x273C607B30F68FAE, 0xE02D4247CAC8D41B, 0xA2DDA3AC6322E288,
  0xBE992B5F8BDB8C5C, 0xFC69CAB42231BACF, 0x3B78E888D80FE17A, 0x7988096371E5D7E9,
  0xF7AA4D1A85996083, 0xB55AACF12C735610, 0x724B8ECDD64D0DA5, 0x30BB6F267FA73B36,
  0x4AC29F2A07BFD00D, 0x08327EC1AE55E69E, 0xCF235CFD546BBD2B, 0x8DD3BD16FD818BB8,
  0x03F1F96F09FD3CD2, 0x41011884A0170A41, 0x86103AB85A2951F4, 0xC4E0DB53F3C36767,
  0xD8A453A01B3A09B3, 0x9A54B24BB2D03F20, 0x5D45907748EE6495, 0x1FB5719CE1045206,
  0x919735E51578E56C, 0xD367D40EBC92D3FF, 0x1476F63246AC884A, 0x568617D9EF46BED9,
  0xE085162AB69D5E3C, 0xA275F7C11F7768AF, 0x6564D5FDE549331A, 0x279434164CA30589,
  0xA9B6706FB8DFB2E3, 0xEB46918411358470, 0x2C57B3B8EB0BDFC5, 0x6EA7525342E1E956,
  0x72E3DAA0AA188782, 0x30133B4B03F2B111, 0xF7021977F9CCEAA4, 0xB5F2F89C5026DC37,
  0x3BD0BCE5A45A6B5D, 0x79205D0E0DB05DCE, 0xBE317F32F78E067B, 0xFCC19ED95E6430E8,
  0x86B86ED5267CDBD3, 0xC4488F3E8F96ED40, 0x0359AD0275A8B6F5, 0x41A94CE9DC428066,
  0xCF8B0890283E370C, 0x8D7BE97B81D4019F, 0x4A6ACB477BEA5A2A, 0x089A2AACD2006CB9,
  0x14DEA25F3AF9026D, 0x562E43B4931334FE, 0x913F6188692D6F4B, 0xD3CF8063C0C759D8,
  0x5DEDC41A34BBEEB2, 0x1F1D25F19D51D821, 0xD80C07CD676F8394, 0x9AFCE626CE85B507
};
void crc64 (const uint8_t *data, const size_t len, uint64_t *crc) {
  uint8_t i;
  for (i = 0; i < len; i++)
  {
    //uint8_t tableIndex = (((uint8_t)(*crc >> 56)) ^ data[i]) & 0xff;
    uint8_t tableIndex = (((uint8_t)(*crc >> 56)) ^ data[i]) & 0xff;
    //*crc = crc64_table[tableIndex] ^ (*crc << 8);
    *crc = (pgm_read_uint64(&crc64_table[tableIndex])) ^ (*crc << 8);
  }
}

const PROGMEM  uint8_t shifts[10][16]  = {
  { 0x4, 0x5, 0x7, 0x6, 0x3, 0x2, 0x0, 0x1, 0xB, 0xA, 0x8, 0x9, 0xC, 0xD, 0xF, 0xE },
  { 0x4, 0xB, 0xB, 0x4, 0xB, 0x4, 0x4, 0xB, 0xA, 0x5, 0x5, 0xA, 0x5, 0xA, 0xA, 0x5 },
  { 0xB, 0x6, 0x0, 0xD, 0xD, 0x0, 0x6, 0xB, 0x6, 0xB, 0xD, 0x0, 0x0, 0xD, 0xB, 0x6 },
  { 0xE, 0x5, 0x9, 0x2, 0x0, 0xB, 0x7, 0xC, 0x3, 0x8, 0x4, 0xF, 0xD, 0x6, 0xA, 0x1 },
  { 0x4, 0xE, 0x1, 0xB, 0xF, 0x5, 0xA, 0x0, 0x3, 0x9, 0x6, 0xC, 0x8, 0x2, 0xD, 0x7 },
  { 0xA, 0x4, 0x7, 0x9, 0x0, 0xE, 0xD, 0x3, 0xE, 0x0, 0x3, 0xD, 0x4, 0xA, 0x9, 0x7 },
  { 0xE, 0x6, 0xE, 0x6, 0xF, 0x7, 0xF, 0x7, 0xD, 0x5, 0xD, 0x5, 0xC, 0x4, 0xC, 0x4 },
  { 0x7, 0x1, 0xB, 0xD, 0xE, 0x8, 0x2, 0x4, 0x4, 0x2, 0x8, 0xE, 0xD, 0xB, 0x1, 0x7 },
  { 0xD, 0xB, 0x0, 0x6, 0x6, 0x0, 0xB, 0xD, 0xA, 0xC, 0x7, 0x1, 0x1, 0x7, 0xC, 0xA },
  { 0xe, 0x1, 0x1, 0xe, 0x1, 0xe, 0xe, 0x1, 0x1, 0xe, 0xe, 0x1, 0xe, 0x1, 0x1, 0xe }
};

uint8_t ApplyPermutationAndShifts(const uint8_t pos, const uint8_t value, const uint8_t nibble) {
  uint8_t shiftbegin = 0, shiftnibble = 0;
  shiftbegin = (pgm_read_byte(&shifts[pos - 1][0]));
  shiftnibble = (pgm_read_byte(&shifts[pos - 1][nibble]));
  return (value ^ (shiftbegin ^ shiftnibble ));
}

char *decode(const char *hex)
{
  uint8_t i;
  char tempChar[2] = {0x00};
  //uint8_t len = strlen(hex);
  char toOut[6];
  char *newString1;//
  newString1 = (char*)alloca(6 * sizeof(char));
  newString1 = (char*)calloc(6, sizeof(char));
  if (newString1 == NULL) {
    free(newString1);
    printf("No memory ");
    delay(100);
    resetFunc();
    return 0;
  }
  char charTemp[3];
  for (i = 0; i < 12; i += 2)
  {
    charTemp[0] = hex[i];
    charTemp[1] = hex[i + 1];
    charTemp[2] = '\0';
    tempChar[0] = (char) (int)strtol(charTemp, NULL, 16);
    tempChar[1] = '\0';
    strncat (newString1, tempChar, 1);
  }
  strcpy(toOut, newString1);
  free(newString1);
  return toOut;
}

void GetOne(const char * uid, const char sector, unsigned char *Key) {
  if (sector == 0) {
    Key[0] = 0x4b;//First Key is Static!!!
    Key[1] = 0x0b;
    Key[2] = 0x20;
    Key[3] = 0x10;
    Key[4] = 0x7c;
    Key[5] = 0xcb;
  } else {
    //declaration
    uint8_t len = 0, i = 0, j = 0, nibble = 0, nibble1 = 0, el_value = 0;
    char *ptrTempChar = (char*)alloca(11 * sizeof(char));
    ptrTempChar = (char*)calloc(11, sizeof(char));
    const char *ptrTemp1 = (char*)alloca(6 * sizeof(char));
    ptrTemp1 = (char*)calloc(6, sizeof(char));
    unsigned char outData[8] = {0x00};
    char charTemp[2] = {0x00};
    char permuted[12] = {0x00};
    char permuted1[12] = {0x00};
    uint64_t out1 = 0;

    sprintf (ptrTempChar, "%s%02x", uid, sector);
    charTemp[0] = ptrTempChar[0];
    charTemp[1] = '\0';
    len = strlen(ptrTempChar);
    nibble1 = strtol(charTemp, NULL, 16) + 1;
    for (i = 1; i < len + 1  ; i++) {
      j = 1;
      el_value = 0;
      el_value = pgm_read_byte(&shifts[i - 1][nibble1 - 1]);

      while (j <= i) {
        if ((i - j) > 0 ) {
          charTemp[0] = ptrTempChar[j];
          nibble = strtol(charTemp, NULL, 16);
          el_value = ApplyPermutationAndShifts(i - j, el_value, nibble);
        }
        j++;
      }
      sprintf (permuted, "%s%x", permuted, el_value);
    }
    sprintf (permuted1, "%s%s\n", "C2", permuted);
    strncpy(ptrTemp1, (decode(permuted1)), 6);//Hex to ascii
    crc64((uint8_t *)ptrTemp1, 6 , &out1); //CRC64 
    
    outData[0] = (uint8_t)(out1 >> 56) & 0xff;  //Cut CRC64 to array[8]
    outData[1] = (uint8_t)(out1 >> 48) & 0xff;
    outData[2] = (uint8_t)(out1 >> 40) & 0xff;
    outData[3] = (uint8_t)(out1 >> 32) & 0xff;
    outData[4] = (uint8_t)(out1 >> 24) & 0xff;
    outData[5] = (uint8_t)(out1 >> 16) & 0xff;
    outData[6] = (uint8_t)(out1 >> 8) & 0xff;
    outData[7] = out1 & 0xff;

    Key[0]=outData[7]; //Inverse CRC64 Output only 6-byte
    Key[1]=outData[6];
    Key[2]=outData[5];
    Key[3]=outData[4];
    Key[4]=outData[3];
    Key[5]=outData[2];
    
    //printf("Debug_%s\n", 6);
    free(ptrTemp1);
    free(ptrTempChar);
    return;
  }
};


