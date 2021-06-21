#include "vex.h"
#include "MysteryGang/RobotConfig.h"

#define ENABLE_COMM_DBG

static int sLeftRight = 0;      //lr: 0=notDetected | 1=left | 2=right | 3=onTarget


void setJetsonDisplay(int lr) {
  sLeftRight = lr;
}

//
// Display various useful information about the Jetson
//
static void dashboardJetson() {
  static int counter = 0;
  static MAP_RECORD  local_map;

  counter++;

  hwBrain.Screen.clearScreen();
  hwBrain.Screen.setCursor(1,1);

  // get last map data
  jetson_comms.get_data( &local_map );

  hwBrain.Screen.print("%d: Hello from dashboard", counter);
  hwBrain.Screen.newLine();

  hwBrain.Screen.print("  Packets   : %d", jetson_comms.get_packets() );
  hwBrain.Screen.newLine();
  hwBrain.Screen.print("  numBoxes  : %d", local_map.boxnum);
  hwBrain.Screen.newLine();
  hwBrain.Screen.print("  numMaps   : %d", local_map.mapnum);
  hwBrain.Screen.newLine();
}

static void dashboardJetson(int ox, int oy, int width, int height) {
  static int32_t     last_data = 0;
  static int32_t     last_packets = 0;
  static int32_t     total_data = 0;
  static int32_t     total_packets = 0;
  static uint32_t    update_time = 0;
  static MAP_RECORD  local_map;
  color grey = vex::color(0x404040);

  #ifdef ENABLE_COMM_DBG
  Brain.Screen.setClipRegion( ox, oy, width, height);
  Brain.Screen.setFont( mono15 );
  // border and titlebar
  Brain.Screen.setPenColor( yellow );
  Brain.Screen.drawRectangle(ox, oy, width, height, black );
  Brain.Screen.drawRectangle( ox, oy, width, 20, grey );

  Brain.Screen.setPenColor( yellow );
  Brain.Screen.setFillColor( grey );
  Brain.Screen.printAt(ox + 10, oy + 15, "Jetson" );
  #endif
  oy += 20;
  
  #ifdef ENABLE_COMM_DBG
  Brain.Screen.setPenColor( white );
  Brain.Screen.setFillColor( black );
  #endif

  // get last map data
  jetson_comms.get_data( &local_map );

  #ifdef ENABLE_COMM_DBG
  Brain.Screen.printAt( ox + 10, oy += 15, "Packets   %d", jetson_comms.get_packets() );
  Brain.Screen.printAt( ox + 10, oy += 15, "Errors    %d", jetson_comms.get_errors() );
  Brain.Screen.printAt( ox + 10, oy += 15, "Timeouts  %d", jetson_comms.get_timeouts() );
  Brain.Screen.printAt( ox + 10, oy += 15, "data/sec  %d             ", total_data );
  Brain.Screen.printAt( ox + 10, oy += 15, "pkts/sec  %d             ", total_packets );
  Brain.Screen.printAt( ox + 10, oy += 15, "boxnum    %d", local_map.boxnum );
  Brain.Screen.printAt( ox + 10, oy += 15, "mapnum    %d", local_map.mapnum );
  Brain.Screen.printAt( ox + 10, oy += 15, "RxCRC     %x", jetson_comms.getPayloadCrc32()); //<cdo>
  Brain.Screen.printAt( ox + 10, oy += 15, "CalcCRC   %x", jetson_comms.getCalcCrc32()); //<cdo>
  #endif

  // once per second, update data rate stats
  if (Brain.Timer.system() > update_time) {
    update_time = Brain.Timer.system() + 1000;
    total_data = jetson_comms.get_total() - last_data;
    total_packets = jetson_comms.get_packets() - last_packets;
    last_data = jetson_comms.get_total();
    last_packets = jetson_comms.get_packets();
  }
  
  Brain.Screen.setFont(mono12);
  float widthI = 0.0;
  float heightI = 0.0;
  float depthI = 0.0;
  bool hasValidTarget = false;
  for (int i=0;i<4;i++) {
    if (i < local_map.boxnum) {
      widthI = local_map.boxobj[i].width / 25.4;
      heightI = local_map.boxobj[i].height / 25.4;
      depthI = local_map.boxobj[i].depth / 25.4;
      #ifdef ENABLE_COMM_DBG
      Brain.Screen.printAt(ox + 10, oy += 12, "box %d: c:%d x:%d y:%d w:%.1f h:%.1f d:%.1f",i,
                           local_map.boxobj[i].classID, //Class ID (0 = Red 1 = Blue 2 = Goal)
                           local_map.boxobj[i].x, //in pixels
                           local_map.boxobj[i].y, //in pixels
                           widthI, heightI, depthI);
      #endif
    }
    else {
      #ifdef ENABLE_COMM_DBG
      Brain.Screen.printAt(ox + 10, oy += 12, "---");
      #endif
    }
  }
  for (int i=0;i<4;i++) {
    if (i < local_map.mapnum) {
      #ifdef ENABLE_COMM_DBG
      Brain.Screen.printAt(ox + 10, oy += 12, "map %d: a:%4d c:%4d X:%.2f Y:%.2f Z:%.1f",i,
                           local_map.mapobj[i].age,
                           local_map.mapobj[i].classID,
                           (local_map.mapobj[i].positionX / -25.4),  // mm -> inches
                           (local_map.mapobj[i].positionY / -25.4),  // mm -> inches
                           (local_map.mapobj[i].positionZ / 25.4)); // mm -> inches
      #endif
    }
    else {
      #ifdef ENABLE_COMM_DBG
      Brain.Screen.printAt( ox + 10, oy += 12, "---");
      #endif
    }
  }

}

//
// Display various useful information about VEXlink
//
static void dashboardVexlink( int ox, int oy, int width, int height ) {
  static int32_t last_data = 0;
  static int32_t last_packets = 0;
  static int32_t total_data = 0;
  static int32_t total_packets = 0;
  static uint32_t update_time = 0;  

  color darkred = vex::color(0x800000);
  color darkgrn = vex::color(0x008000);

  #ifdef ENABLE_COMM_DBG
  Brain.Screen.setClipRegion( ox, oy, width, height);
  Brain.Screen.setFont( mono15 );

  // border and titlebar
  Brain.Screen.setPenColor( yellow );
  Brain.Screen.drawRectangle(ox, oy, width, height, black );
  Brain.Screen.drawRectangle( ox, oy, width, 20 );

  // Link status in titlebar
  Brain.Screen.setPenColor(darkred);
  Brain.Screen.setFillColor(darkred);
  Brain.Screen.drawRectangle( ox+1, oy+1, width-2, 18 );
  Brain.Screen.setPenColor(yellow);
  Brain.Screen.printAt(ox + 10, oy + 15, "VEXlink: Disconnected" );
  #endif
  oy += 20;

  // get last map data
  static MAP_RECORD  local_map;
  jetson_comms.get_data( &local_map );

  char lr = '-';
  if (sLeftRight == 3) {
    lr = 'T';
  }
  else if (sLeftRight == 1) {
    lr = 'L';
  }
  else if (sLeftRight == 2) {
    lr = 'R';
  }
  Brain.Screen.setFillColor(black);
  Brain.Screen.setPenColor(white);
  #ifdef ENABLE_COMM_DBG
  Brain.Screen.printAt( ox + 10, oy += 15, "FrameCnt  %d", local_map.pos.framecnt );
  Brain.Screen.printAt( ox + 10, oy += 15, "Status    %d", local_map.pos.status );
  Brain.Screen.printAt( ox + 10, oy += 15, "x         %d", int(local_map.pos.x) );
  Brain.Screen.printAt( ox + 10, oy += 15, "y         %d", int(local_map.pos.y));
  Brain.Screen.printAt( ox + 10, oy += 15, "z         %d", int(local_map.pos.z));
  Brain.Screen.printAt( ox + 10, oy += 15, "az        %d", int(local_map.pos.az) );
  Brain.Screen.printAt( ox + 10, oy += 15, "el        %d", int(local_map.pos.el));
  Brain.Screen.printAt( ox + 10, oy += 15, "rot       %d", int(local_map.pos.rot));
  Brain.Screen.printAt( ox + 10, oy += 15, "L/R       %c", lr);
  #endif
}

//
// Task to update screen with status
//
int dashboardTask() {
  while (true) {
    // status
    //dashboardJetson();
    dashboardJetson(    0, 0, 280, 240 );
    dashboardVexlink( 279, 0, 201, 240 );
    // draw, at 30Hz
    #ifdef ENABLE_COMM_DBG
    Brain.Screen.render();
    #endif
    //this_thread::sleep_for(16*100);
    this_thread::sleep_for(16);
  }
  return 0;
}
