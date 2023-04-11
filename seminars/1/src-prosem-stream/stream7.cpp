#include <iostream>
#include <iomanip>
#include <fstream>  // fstream
#include <cstdint>
#include <cstring>
using namespace std;

/* Analyze & display WAV file parameters. The program demonstrates the processing of 
 * binary files (headers, subheaders, reading & decoding binary data).
 *
 * The WAV format is a container that is internally divided into chunks of variable lengths.
 * Each chunk starts with the description of the chunk (4 bytes) and the length of the chunk 
 * (another 4 bytes). The interpretation of the bytes in each chunk depends on the particular 
 * chunk type. Our application will only analyze "fmt " chunks - the chunks that describe the 
 * parameters of the stored audio. The other chunks will be skipped (it is easy to skip a 
 * particular chunk, the length of the chunk is given in the header).
 *
 * The file starts with the main header, the main header may (and will) be used to test whether 
 * a file is a valid WAV file.
 *
 * Compile with -std=c++11
 */


// the structure of the global header. The size of the fields is fixed by the WAV standard, therefore, 
// we prefer the fixed-size types from cstdint header (e.g. uint32_t)
struct TGlobal
{
  uint8_t          m_RIFFSignature[4];
  uint32_t         m_TotalSize;
  uint8_t          m_WAVESignature[4];
};

// the structure of a chunk header
struct TChunkHdr
{
  uint8_t          m_ID[4];
  uint32_t         m_Len;
};

// the contents of the format chunk is a structure, 
// the basic version is declared below (there are extended 
// versions of the format, we will only proces the basic variant) 
struct TFormat
{
  uint16_t         m_Format;        // 1 - uncompressed PCM, other = compressed
  uint16_t         m_Channels;      // 1 - mono, 2 - stereo, ...
  uint32_t         m_SampleRate;    // sampling rate in Hz
  uint32_t         m_ReadRate;      // stream read rate in B/s
  uint16_t         m_Align;         // alignment in the data chunk
  uint16_t         m_BitsPerSample; // sampling precision
};
//-------------------------------------------------------------------------------------------------
bool readFormat ( istream & ifs, uint32_t len, TFormat & fmt )
{
  // if the chunk is too small or there was an error reading the chunk
  if ( len < sizeof ( fmt ) 
       || ifs . read ( (char*) &fmt, sizeof ( fmt ) ) . gcount () != sizeof ( fmt ) )
    return false;
  cout << "\tFormat:             " << ( fmt . m_Format == 1 ? "PCM" : "unknown" ) << '\n'
       << "\tChannels:           " << fmt . m_Channels << '\n'
       << "\tSampling frequency: " << fmt . m_SampleRate << " Hz\n"
       << "\tSamples:            " << fmt . m_BitsPerSample << " bits" << endl;
  
  // if the chunk was actually longer than the basic header, skip the remaining bytes in the file
  if ( len > sizeof ( fmt ) )
    ifs . seekg ( len - sizeof ( fmt ), ios::cur );
  
  return true;
}
//-------------------------------------------------------------------------------------------------
bool readData ( ifstream & ifs, uint32_t len, const TFormat & fmt )
{
  // we do not process the audio data in any way. However, the length of the audio data may be 
  // used to compute the duration of the audio
  if ( fmt . m_ReadRate != 0 )
    cout << "\tDuration:           " << fixed << setprecision (3) << (1.0 * len / fmt . m_ReadRate) << " s" << endl;
  ifs . seekg ( len, ios::cur );
  return true;
}
//-------------------------------------------------------------------------------------------------
bool analyzeWAV ( const char * fileName )
{
  cout << fileName << endl;
  ifstream ifs ( fileName, ios::in | ios::binary ); // WAV is a binary file
  
  if ( ifs . fail () )
    return false;
  
  // the WAV file starts with the global header
  TGlobal glob;
  // Try to read the entire header. The bytes are copied into the structure where the fields match 
  // the structure of the WAV header. The read may fail (read fewer bytes than expected), thus the 
  // operation tests the actual number of bytes read.
  if ( ifs . read ( (char*)&glob, sizeof ( glob ) ) . gcount () != sizeof ( glob ) )
    return false;
  
  // test the signatures whether they match the expected WAV format
  if ( memcmp ( glob . m_RIFFSignature, "RIFF", 4 ) 
       || memcmp ( glob . m_WAVESignature, "WAVE", 4 ) )
    return false; 
  
  glob . m_TotalSize -= sizeof ( glob . m_WAVESignature ); // skip the WAVE signature
  
  TChunkHdr chunkHdr;
  TFormat fmt = { 0, 0, 0, 0, 0, 0 };
  
  // following the global header, there are subheaders - chunks.
  // each chunk provides some information (format, audio samples, lyrics, ...)
  while ( glob . m_TotalSize > 0 
          && ifs . read ( (char*) &chunkHdr, sizeof ( chunkHdr ) ) . gcount () == sizeof ( chunkHdr ) )
  {
    if ( ! memcmp ( chunkHdr . m_ID, "fmt ", 4 ) )
    {
      if ( ! readFormat ( ifs, chunkHdr . m_Len, fmt ) ) 
        return false;
    }
    else if ( ! memcmp ( chunkHdr . m_ID, "data", 4 ) )
    {
      if ( ! readData ( ifs, chunkHdr . m_Len, fmt ) )
        return false;
    }
    else
    {
      // an unknown chunk. We do not understand the chunk, we just skip it
      // the length of the chunk is actually added to the current posidion, i.e. ios::cur is used
      ifs . seekg ( chunkHdr . m_Len, ios::cur );
    }
    glob . m_TotalSize -= sizeof ( chunkHdr ) + chunkHdr . m_Len;
  }

  // we consider the file corerct only if we have read the entire contents
  if ( glob . m_TotalSize != 0 || ifs . get () != EOF )
    return false;
  
  // we may (but do not have to) explicitly close the input file. If we do not close it, 
  // the destructor of ifs would close the file anyway
  return true;
}
//-------------------------------------------------------------------------------------------------
int main ( int argc, char * argv [] )
{
  if ( argc < 2 )
  {
    cout << "Usage: analyze <WAV file(s)>" << endl;
    return 1;
  }

  for ( int i = 1; i < argc; i ++ )
    if ( ! analyzeWAV ( argv[i] ) )
      cerr << "File read error." << endl;
  return 0;
}
