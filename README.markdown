# Xenophobic Xylophone
## 2013 Ultimate Ascent

[Description with picture goes here]

## Summary of LabVIEW Code
  - Two joysticks for driving, one for shooting
  - Tank drive with shifting
  - PID Control for maintaining proper shooter speed
  - Lead screw for adjusting angle of elevation of shooter
  - Servo controlled camera pan and tilt
  - Two pneumatic pistons for climbing the first level
  - Two limit switches to activate the pistons

## Summary of Arduino Code (Mammoth Mallet)
  - Read from ADXL345 on an I2C bus and calculate angles based on force vectors  
  - Measure RPM of shooter
  - TCP Server via Ethernet
  - Transmit data collected to dash and to cRIO on port 1180

### Images of code coming soon!

- - -

Code created by [Mason Tran](mailto:mason@Lmnotran.com), [Ryan Glaser](mailto:rglaser12@gmail.com), and [Ryan Zoeller](mailto:rtzoeller@hotmail.com), members of [FRC Team 2169 KING TeC](http://kingtec2169.com/)