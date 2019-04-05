#pragma once

// ServoEnum Usage
// 	 ServoEnum servo = ServoEnum::POSF;
//		or
//   int e = static_cast<int>(ServoEnum::POSF);

enum class ServoEnum {
	POSF = 0,	// Summation of position feedback pulse data (POSF); 1 = Detection unit(1 pulse)
	ERR,	// Position error (ERR);  1 = Detection unit(1 pulse) 
	VCMD,	// Velocity command (VCMD);  16,777,216  = 3750min-1 (In case of shift = 9: 32768 = 3750min - 1)
	TCMD,	// Torque command (TCMD); 7282  = The maximum current value of amplifier (A) 
	SPEED,	// Speed feedback signal (SPEED); 
	DTRQ,	// Estimated disturbance torque (DTRQ);  7,282  = The maximum current value of amplifier (A)
	SYNC,	// Synchronous error on rigid tapping (SYNC); 1 =  = 1/4096 rotation of spindle 
	OVCLV,	// OVC simulation data (OVCLV); 1 = 1%
	IEFF,	// Effective current (IEFF); 6,554 = The maximum current value of amplifier (A) 
	VDC,	// DC link voltage(VDC); 8,192 = 640V (200V) or 1280V (400V) 
	SFERR,	// Semi-Full error(SFERR); 1 = Detection unit(1 pulse) 
	ALGCMD,	// Analog signal command(ALGCMD); 32,767 = 100% (Maximum analog signal command)
	ALGFB,	// Analog signal feedback(ALGFB); 32,767 = = 100% (Maximum analog signal feedback)
	IR,		// R-phase current(IR); 6,554  = The maximum current value of amplifier (A) 
	IR1,	// R-phase current(PDM Slave1)(IR1); 6,554  = The maximum current value of amplifier (A) 
	IR2,	// R-phase current(PDM Slave2)(IR2); 6,554  = The maximum current value of amplifier (A) 
	IR3,	// R-phase current(PDM Slave3)(IR3); 6,554  = The maximum current value of amplifier (A) 
	IR4,	// R-phase current(PDM Slave4)(IR4); 6,554  = The maximum current value of amplifier (A) 
	IS,		// S-phase current(IS); 6,554  = The maximum current value of amplifier (A) 
	IS1,	// S-phase current(PDM Slave1)(IS1); 6, 554 = The maximum current value of amplifier(A)
	IS2,	// S-phase current(PDM Slave2)(IS2); 6, 554 = The maximum current value of amplifier(A)
	IS3,	// S-phase current(PDM Slave3)(IS3); 6, 554 = The maximum current value of amplifier(A)
	IS4,	// S-phase current(PDM Slave4)(IS4); 6, 554 = The maximum current value of amplifier(A)
	ID,		// Reactive current(ID); 8027 = The maximum current value of amplifier (A) 
	IQ,		// Active current (IQ); 8027 = The maximum current value of amplifier (A) 
	MTTMP,	// Motor winding temperature(MTTMP); 1 = 1 degrees centigrade 
	PCTMP,	// Pulsecoder temperature(PCTMP);  1 = 1 degrees centigrade 
	VCOUT,	// Velocity command without FF(VCOUT); Same as VCMD
	VCC0,	// Advanced velocity command(VCC0); Internal unit(not open)
	BLACL,	// Backlash acceleration amount(BLACL); Internal unit(not open)
	ABS,	// Absolute position detected internal pulse coder(ABS);  1 = 1/2^21 rotation
	FREQ,	// Vibration frequency(FREQ); 1 = 1Hz
	FRTCM,	// Vibration torque command(FRTCM); 7,282 = The maximum current value of amplifier (A) 
	VDC2,	// DC link voltage(VDC); 8,192 = 640V (200V) or 1280V (400V) 
	ACC,	// Acceleration feedback in direction specified by No.2277#7,6,5(ACC); In case of αiGS0.1-3D (0.1G) 512 =  = 0.1G = 0.98m/s^2;  In case of αiGS1-3D (1G) 4096 =  = 1G = 9.8m/s^2  
	ACC1,	// 1st direction acceleration feedback(ACC1); In case of αiGS0.1-3D (0.1G) 512 =  = 0.1G = 0.98m/s^2;  In case of αiGS1-3D (1G) 4096 =  = 1G = 9.8m/s^2  
	ACC2,	// 2nd direction acceleration feedback(ACC2); In case of αiGS0.1-3D (0.1G) 512 =  = 0.1G = 0.98m/s^2;  In case of αiGS1-3D (1G) 4096 =  = 1G = 9.8m/s^2  
	ACC3,	// 3rd direction acceleration feedback(ACC3); In case of αiGS0.1-3D (0.1G) 512 =  = 0.1G = 0.98m/s^2;  In case of αiGS1-3D (1G) 4096 =  = 1G = 9.8m/s^2  
	FRVCM,	// Vibration velocity command(FRVCM); 16,777,216 = 3750min-1(In case of shift = 9: 32768 = 3750min - 1)
	BLCMP,	// Backlash compensation(BLCMP); 1 = Detection unit(1 pulse)
	BLAC1,	// Backlash acceleration amount 1(BLAC1);  Internal unit(not open) 
	BLAC2,	// Backlash acceleration amount 2(BLAC2); Internal unit(not open)
	SMTBKL, // Smart backlash compensation (position compensation amount)(SMTBKL);  1 = Detection unit(1 pulse) 
	SMTTRQ, // Estimated load torque for Smart backlash compensation(SMTTRQ); 7282 = The maximum current value of amplifier (A) 
	SMTBAC,	// Compensation amount of smart backlash (for full-closed)(SMTBAC); 1 = Detection unit(1 pulse) 
	ALGOFS,	// Analog signal offset(ALGOFS);  In case of Voltage input(AIM) -32000 to 32000 = 10V to +10V; In case of current input(AMU) -32768 to 32767 = 0 to 25mA
	ALGOF2	// Analog signal offset(ALGOF2);  In case of Voltage input(AIM) -32000 to 32000 = 10V to +10V; In case of current input(AMU) -32768 to 32767 = 0 to 25mA

};
