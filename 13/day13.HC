"Advent of Code Day 13 by Matthias Quintern in HolyC\n";

//
//ENTRYCLASS
//
#define EMPTY 0
#define PACKET 1
#define NUMBER2

/* 
if type is PACKET, packet is pointer to packet
if type is NUMBER, val is value
*/
//PACKETCLASS
#define MAX_PACKET_LENGTH 15
class Packet {
  I32 type;
  // only if NUMBER
  U8 val[2];
  //only if PACKET
  U16 entryCount;
  Packet* packets[MAX_PACKET_LENGTH];
};


//PACKETMEMBERFUNCTIONS
U0 initPacket(Packet* p) {
  p->entryCount = 0;
  I64 i;
  for (i=0; i<MAX_PACKET_LENGTH; i+=1) {
    p->packets[i]=NULL;
  }
  p->val[0]='0';
  p->val[1] ='0';
  p->type = EMPTY;

}
U0 resetPacket(Packet* p) {
  if (p->type==PACKET){
    I64 i;
    for (i=0; i<p->entryCount; i+=1) {
      resetPacket(p->packets[i]);
      Free(p->packets[i]);
      p->packets[i] =NULL;
    }    
  }
  else if (p->type ==NUMBER) {
    p->val[0]='0';
    p->val[1] ='0';
  }
  p->type =EMPTY;
  p->entryCount =0;
}


U0 printPacket(Packet* p) {
  if (p->type ==NUMBER) {
    if (p->val[0] =='0') "%c",p->val[1];
    else "%c%c",p->val[0], p->val[1]; 
  }
  else if (p->type ==PACKET) {
    "[";
    I64 i;
    for (i=0; i<p->entryCount; i+=1) {
      printPacket(p->packets[i]);
      ",";
    }
    "]";
  }
  else "X";
}

Packet*nextSubPacket(Packet* p) {
  // add and return new subpacket
  p->type =PACKET;
  p->packets[p->entryCount] =MAlloc(sizeof(Packet));
  Packet* new =p->packets[p->entryCount];
  initPacket(new);
  p->entryCount +=1;
  return new;
}

I32 RIGHT_ORDER=1;
I32 WRONG_ORDER=-1;
I32 CANT_SAY=0;
I32 compareValues(U8* lhs, U8* rhs) {
  if (lhs[0] > rhs[0]) return WRONG_ORDER;
  if (lhs[0] < rhs[0]) return RIGHT_ORDER;
  if (lhs[1] > rhs[1])return WRONG_ORDER;
  if (lhs[1] < rhs[1]) return RIGHT_ORDER;
  return CANT_SAY;
}

I32 comparePackets(Packet* lhs, Packet* rhs) { 
  //">>>Comparing lhs type=%d <-> rhs type=%d\n", lhs->type, rhs->type;
  //"    "; printPacket(lhs);"\n    "; printPacket(rhs);"\n";
  if (lhs->type != EMPTY && rhs->type ==EMPTY)      // [...]/n vs []
    return WRONG_ORDER;
  if (lhs->type ==EMPTY&&rhs->type !=EMPTY)      //[] vs [...]/n
    return RIGHT_ORDER;
  
  if (lhs->type ==NUMBER && rhs->type ==NUMBER) {  // n vs n
    return compareValues(lhs->val, rhs->val);
  }
  if (lhs->type ==NUMBER&& rhs->type ==PACKET) {  	  
    return comparePackets(lhs, rhs->packets[0]);     // n vs [...][0]
  }
  if (lhs->type ==PACKET&&rhs->type ==NUMBER) {
    return comparePackets(rhs, lhs) * (-1);         // [...] vs n
  }
  if (lhs->type ==PACKET&&rhs->type ==PACKET) {  // [...] vs [...]
    U16 i;
    I32 result;
    for (i=0; i<lhs->entryCount && i<rhs->entryCount; i+=1) {
      result = comparePackets(lhs->packets[i], rhs->packets[i]);
      if (result !=CANT_SAY) return result;
    }
    if (lhs->entryCount > rhs->entryCount) return WRONG_ORDER;
    if (lhs->entryCount < rhs->entryCount) return RIGHT_ORDER;  
    return CANT_SAY;
  }
  "ERROR:Failed to compare Packets\n";
}

U0 makeDivider(Packet* p, U8 val){
  initPacket(p);
  Packet* sub =nextSubPacket(p);
  sub->type =NUMBER;
  sub->val[1] =val;
  sub->val[0] ='0';
}



// PARSEFILE
/*
 [ and , begin new entries -> nextSubpacket
 ] always ends a packet
*/
#define MAX_PACKET_DEPTH 10
U64 parseFile(U8* fileBuf, I64 fileSize) {
  U64 indexSum =0;
  Packet d2;
  Packet d6;
  makeDivider(&d2, '2');
  makeDivider(&d6, '6');
  U32 d2i =1;
  U32 d6i =2;
  Packet lhs, rhs;
  initPacket(&rhs);
  initPacket(&lhs);
  Packet*packets[MAX_PACKET_DEPTH];  //stack for packets
  packets[0] =&lhs;
  Packet* curP =packets[0];
  I32 depth= 0;  // sub-packet nr: -1 not in any package, 0 package, 1 first subpackage
  U32 pairNr=1;
  U32 digitNr =0;
  I64 i;
  for (i =0;i < fileSize; i+=1) {
    //Print("parseFile:char nr %d is %c\n", i, fileBuf[i]); 
    switch (fileBuf[i]) {
      case '[':
	//">>>Subpacket Begin\n";       
	depth += 1;
        packets[depth] =nextSubPacket(curP);
        curP=packets[depth]; 
	break;
      case']':
 	//">>>Packet End\n";
	depth -= 1;
	curP = packets[depth];
	
	if(depth == 0) {  // packet end
	  if (packets[0] ==&lhs) { // first packet end
	    //"finished lhs\n";
	    packets[0] =&rhs;  // wait for second packet begin
	    curP= packets[0];
	  }
	  else {  // second packet end
	    //"lhs:";printPacket(&lhs);"\n";
	    //"rhs:";printPacket(&rhs);"\n";
	    //TODO:compare packets
	    U32 result =comparePackets(&lhs, &rhs);
	    switch(result) {
	      case CANT_SAY:
		Print("Can not determine order for pair nr %d\n", pairNr);
		break;
	      case RIGHT_ORDER:
		Print("Right order for pair nr %d\n", pairNr);
		indexSum += pairNr;
	  	break;
	      case WRONG_ORDER:
		Print("Wrong order for pair nr %d\n", pairNr);
		break;
	    }  //switch
	    pairNr += 1;
  	    // task 2
            result =comparePackets(&lhs, &d2);
            if (result ==RIGHT_ORDER) {
              d6i += 1;
	      d2i += 1;
	    } else {
              result =comparePackets(&lhs, &d6);
              if (result ==RIGHT_ORDER) { d6i += 1; }
	    }       

            result =comparePackets(&rhs, &d2);
            if (result ==RIGHT_ORDER) {
              d6i += 1;
	      d2i += 1;
	    } else {
              result =comparePackets(&rhs, &d6);
              if (result ==RIGHT_ORDER) { d6i += 1; }
	    }

 
	    resetPacket(&lhs);
	    resetPacket(&rhs);
	    packets[0] =&lhs;
	    curP=packets[0];
	    //if (pairNr == 15) return indexSum;
	  }
	}
	break;
      case '0'...'9':
	//"Number: %c\n", fileBuf[i];
	curP->type =NUMBER;
        if (digitNr =0) {
	  curP->val[1] =fileBuf[i];
	  digitNr += 1;
	}
  	else {
  	  curP->val[0] = curP->val[1];
  	  curP->val[1]= fileBuf[i];
	  digitNr =0;
	}
	break;
      case',':  //finished writing to entry
	packets[depth] =nextSubPacket(packets[depth-1]);
        curP =packets[depth];
	break;
      case '\n':
	break;
      default:
        "Did not match character %c", fileBuf[i];
    }
    //"curP=%P", curP;
    // debug
    /*
    if (packets[0] ==&lhs) {
      "After char %c: lhs packet, depth=%d, entryCount=%d\n", fileBuf[i], depth, packets[depth]->entryCount;
    }
    else { 
      "After char %c: rhs packet, depth=%d, entryCount=%d\n", fileBuf[i], depth, packets[depth]->entryCount;
    }
    */
  }
  resetPacket(&lhs);
  resetPacket(&rhs);
  "Index of divider 2: %d\n", d2i;
  "Index of divider 6: %d\n", d6i;
  "Decoder key: %d\n", d2i * d6i;
  return indexSum;
}
//Main i guess
U8* filepath ="D:/Home/aoc/signal.TXT";

//U8* filepath ="D:/Home/aoc/testsignal.TXT";
//U8* filepath ="D:/Home/aoc/test.TXT";

/*CFile*file =FOpen(filepath, "r");
I64 fileSize =FSize(file);
Print("Opened file %s with size :%d\n", filepath, fileSize);
U8* fileBuf =MAlloc(fileSize);
FBlkRead(file, fileBuf, , fileSize);
FClose(file);
*/
I64 fileSize = 0;
U8* fileBuf =FileRead(filepath, &fileSize);
Print("Opened file %s with size: %d\n", filepath, fileSize);

U64 indexSum =0;
indexSum =parseFile(fileBuf, fileSize);


Print("Indices Sum for packets in correct order is %d\n", indexSum);
Free(fileBuf);


"End of program\n";