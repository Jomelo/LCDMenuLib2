/* ===================================================================== *
 *                                                                       *
 * Conditions to show or hide a menu element on the display              *
 *                                                                       *
 * ===================================================================== *
 */

// *********************************************************************
boolean COND_hide(void)  // hide a menu element
// *********************************************************************
{
  return false;  // hidden
}

// *********************************************************************
boolean COND_a(void)
// *********************************************************************
{
  return g_condetion_a;
}

// *********************************************************************
boolean COND_not_a(void)
// *********************************************************************
{
  // invert condetion a "!"
  return !g_condetion_a;
}
