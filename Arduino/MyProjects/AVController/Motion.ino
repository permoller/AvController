
Mode handleMotion(Mode currentMode) {

  // if we just detected motion the signal is most likely still high, so don't check again right away to avoid the delay when reading the sensor the second time
  if((motionDetectedTime + SECS_PER_MIN) > currentTime)
    return currentMode;
  
  bool motionDetected = digitalRead(pirSensorPin) == HIGH;
  if(!motionDetected)
    return currentMode;
  delay(100);
  motionDetected = digitalRead(pirSensorPin) == HIGH;
  if(!motionDetected)
    return currentMode;

  motionDetectedTime = currentTime;

#ifdef debugMotion
  pl("Motion detected");
#endif

  if(currentMode == Mode_Off)
    return Mode_Standby;
  return currentMode;
}
