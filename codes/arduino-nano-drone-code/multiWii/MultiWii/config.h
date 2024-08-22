#ifndef CONFIG_H_
#define CONFIG_H_
#define QUADX
#define MINTHROTTLE 1150 // 
#define MAXTHROTTLE 2000
#define MINCOMMAND  1000
#define I2C_SPEED 100000L     //100kHz normal mode, this value must be used for a genuine WMP
#define NANOWII         // the smallest multiwii FC based on MPU6050 + pro micro based proc <- confirmed by Alex
#define GY_521          // Chinese 6  DOF with  MPU6050, LLC 
#define PID_CONTROLLER 1  //PID
#define YAW_DIRECTION 1 //yaw axis direction
#define ONLYARMWHENFLAT //prevent the copter from arming when the copter is tilted
#define ALLOW_ARM_DISARM_VIA_TX_YAW
#define CAM_TIME_HIGH 1000   // the duration of HIGH state servo expressed in ms
#define FLAPPERON_EP   { 1500, 1700 } // Endpooints for flaps on a 2 way switch else set {1020,2000} and program in radio.
#define FLAPPERON_INVERT { -1, 1 }    // Change direction om flapperons { Wing1, Wing2 }
#define COLLECTIVE_PITCH      THROTTLE  /* Channel to control CollectivePitch */
#define COLLECTIVE_RANGE { 80, 0, 80 }// {Min%, ZeroPitch offset from 1500, Max%}.  
/* Limit the range of Collective Pitch. 100% is Full Range each way and position for Zero Pitch */
#define YAWMOTOR                 0       // If a motor is used as YAW Set to 1 else set to 0.
/* Servo mixing for heli 120 {Coll,Nick,Roll} */
#define SERVO_NICK   { +10, -10,  0 }
#define SERVO_LEFT   { +10, +5, +10 } 
#define SERVO_RIGHT  { +10, +5, -10 } 
#define CONTROL_RANGE   { 100, 100 }      //  { ROLL,PITCH }  /* Limit Maximum controll for Roll & Nick  in 0-100% */
#define SERIAL_SUM_PPM           THROTTLE,YAW,PITCH,ROLL,AUX1,AUX2,AUX3,AUX4,8,9,10,11 // BoykaCopter
#define SBUS_MID_OFFSET 988 //SBUS Mid-Point at 1500
/* This is the speed of the serial interfaces */
#define SERIAL0_COM_SPEED 115200
#define SERIAL1_COM_SPEED 115200
#define SERIAL2_COM_SPEED 115200
#define SERIAL3_COM_SPEED 115200
#define INTERLEAVING_DELAY 3000 /* interleaving delay in micro seconds between 2 readings WMP/NK in a WMP+NK config*/
#define NEUTRALIZE_DELAY 100000 /* when there is an error on I2C bus, we neutralize the values during a short time*/
#define MPU6050_LPF_10HZ
#define AP_MODE 40  /* Temporarily Disables GPS_HOLD_MODE*/ // Create a deadspan for GPS.
#define FAILSAFE_DELAY     10                     // Guard time for failsafe activation after signal lost. 1 step = 0.1sec - 1sec in example
#define FAILSAFE_OFF_DELAY 200                    // Time for Landing before motors stop in 0.1sec. 1 step = 0.1sec - 20sec in example
#define FAILSAFE_THROTTLE  (MINTHROTTLE + 200)    // (*) Throttle level used for landing - may be relative to MINTHROTTLE - as in this case
#define FAILSAFE_DETECT_TRESHOLD  985
#define GPS_BAUD   57600  // avoid using 115200 baud because with 16MHz arduino the 115200 baudrate have more than 2% speed error (57600 have 0.8% error)
/* indicate a valid GPS fix with at least 5 satellites by flashing the LED  - Modified by MIS - Using stable LED (YELLOW on CRIUS AIO) led work as sat number indicator 
- No GPS FIX -> LED blink at speed of incoming GPS frames
- Fix and sat no. bellow 5 -> LED off
- Fix and sat no. >= 5 -> LED blinks, one blink for 5 sat, two blinks for 6 sat, three for 7 ... */
#define GPS_LED_INDICATOR
#define NAV_CONTROLS_HEADING       true      // copter faces toward the navigation point, maghold must be enabled for it
#define NAV_TAIL_FIRST             false     // true - copter comes in with tail first 
#define NAV_SET_TAKEOFF_HEADING    true      // true - when copter arrives to home position it rotates it's head to takeoff direction
#define MAG_DECLINATION  0.0f
#define GPS_LEAD_FILTER                      // Adds a forward predictive filterig to compensate gps lag. Code based on Jason Short's lead filter implementation
#define GPS_WP_RADIUS              200       // if we are within this distance to a waypoint then we consider it reached (distance is in cm)
#define NAV_SLEW_RATE              30        // Adds a rate control to nav output, will smoothen out nav angle spikes
#define LCD_SERIAL_PORT 0    // must be 0 on Pro Mini and single serial boards; Set to your choice on any Mega based board
/* keys to navigate the LCD menu */
#define LCD_MENU_PREV 'p'
#define LCD_MENU_NEXT 'n'
#define LCD_VALUE_UP 'u'
#define LCD_VALUE_DOWN 'd'
#define LCD_MENU_SAVE_EXIT 's'
#define LCD_MENU_ABORT 'x'
#define VBATSCALE       131 // (*) (**) change this value if readed Battery voltage is different than real voltage
#define VBATNOMINAL     126 // 12,6V full battery nominal voltage - only used for lcd.telemetry
#define VBATLEVEL_WARN1 107 // (*) (**) 10,7V
#define VBATLEVEL_WARN2  99 // (*) (**) 9.9V
#define VBATLEVEL_CRIT   93 // (*) (**) 9.3V - critical condition: if vbat ever goes below this value, permanent alarm is triggered
#define NO_VBAT          16 // Avoid beeping without any battery
#define PSENSORNULL 510 /* (*) hard only: set to analogRead() value for zero current*/
#define PINT2mA 132     /* (*) hard: one integer step on arduino analog translates to mA (example 4.9 / 37 * 1000)*/
#define ALT_HOLD_THROTTLE_NEUTRAL_ZONE    50  /* defines the neutral zone of throttle stick during altitude hold*/
#define BOARD_NAME "MultiWii   V-.--"
#define NO_FLASH_CHECK
#define MIDRC 1500
#define SERVO_RFR_50HZ
#define MEGA_HW_PWM_SERVOS
#define SERVO_RFR_RATE  50    // In Hz, you can set it from 20 to 400Hz, used only in HW PWM mode for mega*/
/* to calibrate all ESCs*/
#define ESC_CALIB_LOW  MINCOMMAND
#define ESC_CALIB_HIGH 2000
#define LCD_TELEMETRY_FREQ 23       // to send telemetry data over serial 23 <=> 60ms <=> 16Hz (only sending interlaced, so 8Hz update rate)
#define LCD_TELEMETRY_AUTO_FREQ  967// to step to next telemetry page 967 <=> 3s
#define PSENSOR_SMOOTH 16           // len of averaging vector for smoothing the PSENSOR readings; should be power of 2; set to 1 to disable
#define VBAT_SMOOTH 16              // len of averaging vector for smoothing the VBAT readings; should be power of 2; set to 1 to disable
#define RSSI_SMOOTH 16              // len of averaging vector for smoothing the RSSI readings; should be power of 2; set to 1 to disable
#define DISABLE_POWER_PIN /* disable use of the POWER PIN (allready done if the option RCAUXPIN12 is selected) */
#endif /* CONFIG_H_ */