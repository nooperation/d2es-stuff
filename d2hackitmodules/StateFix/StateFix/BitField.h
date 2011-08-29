#pragma once

class BitField
{
public:
   BitField(void* data, int size);
   BitField();

   void SetData(void* data, int size);

   int ReadBits(int position, int span) const;
   void WriteBits(int position, int span, int value);
   bool ReadBit(int position) const;
   void WriteBit(int position, bool value);

private:
   static void ConvertBitPosition(int position, int* byte, int* bit);

   unsigned char* _data;
   int _size;
};