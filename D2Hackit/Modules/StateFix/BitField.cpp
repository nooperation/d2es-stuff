#include <assert.h>
#include "BitField.h"

BitField::BitField(void* data, int size) :
   _data(0),
   _size(0)
{
   SetData(data, size);
}

BitField::BitField() :
   _data(0),
   _size(0)
{

}

void BitField::SetData(void* data, int size)
{
   _data = reinterpret_cast<unsigned char*>(data);
   _size = size;
}

int BitField::ReadBits(int position, int span) const
{
   int value = 0;

   for (int i = 0; i < span; i++)
   {
      if (ReadBit(position + i))
      {
         value |= 1 << i;
      }
   }
   
   return value;
}

void BitField::WriteBits(int position, int span, int value)
{
   for (int i = 0; i < span; i++)
   {
      bool bit = (value & (1 << i)) != 0;
      WriteBit(position + i, bit);
   }
}

bool BitField::ReadBit(int position) const
{
   int byte = 0;
   int bit = 0;

   ConvertBitPosition(position, &byte, &bit);

   if (byte >= _size)
   {
      // reading past end of data is bad
//      assert(false);
  //    return false;
   }

   return (_data[byte] & (1 << bit)) != 0;
}

void BitField::WriteBit(int position, bool value)
{
   int byte = 0;
   int bit = 0;

   ConvertBitPosition(position, &byte, &bit);

   if (byte >= _size)
   {
      // writing past end of data is bad
      assert(false);
   }

   else if (value)
   {
      // setting the bit
      _data[byte] |= (1 << bit);
   }

   else
   {
      // clearing the bit
      _data[byte] &= ~(1 << bit);
   }
}

void BitField::ConvertBitPosition(int position, int* byte, int* bit)
{
   if (byte != 0)
   {
      *byte = position / 8;
   }

   if (bit != 0)
   {
      *bit = position % 8;
   }
}