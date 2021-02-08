float fadd(float a, float b)
{
   float result;
   __asm__ volatile (
       "mtv    %1, S000\n"
       "mtv    %2, S001\n"
       "vadd.s S002, S000, S001\n"
       "mfv    %0, S002\n"
       : "=r"(result) : "r"(a), "r"(b));
   return result;
}
float fsub(float a, float b)
{
   float result;
   __asm__ volatile (
       "mtv    %1, S000\n"
       "mtv    %2, S001\n"
       "vsub.s S002, S000, S001\n"
       "mfv    %0, S002\n"
       : "=r"(result) : "r"(a), "r"(b));
   return result;
}
float fmul(float a, float b)
{
   float result;
   __asm__ volatile (
       "mtv    %1, S000\n"
       "mtv    %2, S001\n"
       "vmul.s S002, S000, S001\n"
       "mfv    %0, S002\n"
       : "=r"(result) : "r"(a), "r"(b));
   return result;
}
float fdiv(float a, float b)
{
   float result;
   __asm__ volatile (
       "mtv    %1, S000\n"
       "mtv    %2, S001\n"
       "vrcp.s  S001, S001\n"
       "vmul.s  S000, S000, S001\n"
       "mfv    %0, S000\n"
       : "=r"(result) : "r"(a), "r"(b));
   return result;
}