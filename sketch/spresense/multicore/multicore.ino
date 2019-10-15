/*
    Main,sub common
*/



#include <MP.h>

#define sub_1  1
#define sub_2  2
#define MEMSIZE (32 * 1024)

void loop()
{
}


/*
    Main.ino
*/


#if (SUBCORE == 1)

#include <MP.h>

void setup()
{
  int8_t msgid;
  void *addr;

  MP.begin();
  printf("subcore1 begin\n");

  /* Receive from MainCore */
  MP.Recv(&msgid, &addr);
  printf("SharedMemory Address=@%08x  (sub1)\n", (uint32_t)addr);

  /* memory fill */
  memset(addr, 0xaa, MEMSIZE);

  /* Send address to MainCore */
  MP.Send(msgid, addr);

//  subcore can't get Common Memory
//  uint8_t *addrsub1 = (uint8_t *)MP.AllocSharedMemory(MEMSIZE);
//  printf("subcore get CommonRam =@%08x  (main)\n", (uint32_t)addrsub1);

}


#elif (SUBCORE == 2)
void setup()
{
  int8_t msgid;
  void *addr;

  MP.begin();
  printf("subcore2 begin\n");

  /* Receive from MainCore */
  MP.Recv(&msgid, &addr);
  printf("SharedMemory Address=@%08x  (sub2)\n", (uint32_t)addr);

  /* memory fill */
  memset(addr, 0x55, MEMSIZE);

  /* Send address to MainCore */
  MP.Send(msgid, addr);
}

#else //main core

void setup()
{

  int usedMem, freeMem, largestFreeMem;


  Serial.begin(115200);
  while (!Serial);


  MP.GetMemoryInfo(usedMem, freeMem, largestFreeMem);
  printf("Used:%4d [KB] / Free:%4d [KB] (Largest:%4d [KB])\n",
         usedMem / 1024, freeMem / 1024, largestFreeMem / 1024);


  /* Boot SubCore */
  MP.begin(1);
  //  MP.begin(2);

  MP.GetMemoryInfo(usedMem, freeMem, largestFreeMem);
  printf("Used:%4d [KB] / Free:%4d [KB] (Largest:%4d [KB])\n",
         usedMem / 1024, freeMem / 1024, largestFreeMem / 1024);

  /* Allocate Shared Memory */
  uint8_t *addr1 = (uint8_t *)MP.AllocSharedMemory(MEMSIZE);
  printf("SharedMemory Address1=@%08x  (main)\n", (uint32_t)addr1);



  MP.begin(2);
  uint8_t *addr2 = (uint8_t *)MP.AllocSharedMemory(MEMSIZE);
  printf("SharedMemory Address1=@%08x  (main)\n", (uint32_t)addr2);

  MP.GetMemoryInfo(usedMem, freeMem, largestFreeMem);
  printf("Used:%4d [KB] / Free:%4d [KB] (Largest:%4d [KB])\n",
         usedMem / 1024, freeMem / 1024, largestFreeMem / 1024);


  /* memory fill */
  memset(addr1, 0x55, MEMSIZE);
  memset(addr1, 0xaa, MEMSIZE);

  /* Send shared memory address to SubCore */
  int8_t msgid = 10;
  MP.Send(msgid, addr1, sub_1);
  MP.Send(msgid, addr2, sub_2);

  /* Receive from SubCore */
  void *raddr;
  MP.Recv(&msgid, &raddr, sub_1);
  printf("subcore1 return Address=@%08x  (sub1)\n", (uint32_t)raddr);
  /* shared memory check */
  int i;
  for (i = 0; i < MEMSIZE; i++) {
    if (addr1[i] != 0xaa) {
      printf("Error: @%08x\n", (uint32_t)&addr1[i]);
      while (1);
    }
  }
  printf("sub1 SharedMemory Check: OK!\n");

  MP.Recv(&msgid, &raddr, sub_2);
  printf("subcore1 return Address=@%08x  (sub1)\n", (uint32_t)raddr);
  /* shared memory check */
  for (i = 0; i < MEMSIZE; i++) {
    if (addr2[i] != 0x55) {
      printf("Error: @%08x\n", (uint32_t)&addr2[i]);
      while (1);
    }
  }
  printf("sub2 SharedMemory Check: OK!\n");

  /* Free Shared Memory */
  MP.FreeSharedMemory(addr1);
  MP.FreeSharedMemory(addr2);
}
#endif
