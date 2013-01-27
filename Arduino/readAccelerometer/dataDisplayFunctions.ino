void sPrintAccel()  {
  /* This outputs data in the format:
  *
  *  X: __ Y: __ Z: __
  *        or
  *  X: 23 Y: -19 Z: 127
  *
  */
  Serial.print("X: ");
  Serial.print( x );
  Serial.print(" Y: ");
  Serial.print( y );
  Serial.print(" Z: ");
  Serial.println( z );
}
