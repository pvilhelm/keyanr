/* Definiera ett makro swap(t, x, y) som byter plats på två argument av typen t.
 * (Blockstrukturer är till hjälp)
 */

#define swap(t,x,y) do { t tmp = x; x = y; y = tmp; } while(0)
