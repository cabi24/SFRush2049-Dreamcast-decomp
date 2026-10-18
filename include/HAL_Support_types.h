/* HAL_IntLog2: target R4 unsigned32 bitmask input, unsigned-byte R0 return.
 * Target8C02ECD4..8C02ECF4; caller AudioInit3BAE..3BB6 zero-extends R0.
 * No target structures, externals, initialized data, or BSS are needed.
 */
unsigned char HAL_IntLog2(unsigned int num);
