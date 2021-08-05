/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 17.12
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HMESSAGE
#define HCPP_HMESSAGE

namespace HalconCpp
{

// Represents an instance of a data container to be sent via message queues.
class LIntExport HMessage : public HToolBase
{

public:

  // Copy constructor
  HMessage(const HMessage& source) : HToolBase(source) {}

  // Create HMessage from handle, taking ownership
  explicit HMessage(Hlong handle);

  // Set new handle, taking ownership
  void SetHandle(Hlong handle);



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // create_message: Create a new empty message.
  explicit HMessage();




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Query message parameters or information about the message.
  HTuple GetMessageParam(const HString& GenParamName, const HTuple& Key) const;

  // Query message parameters or information about the message.
  HTuple GetMessageParam(const HString& GenParamName, const HString& Key) const;

  // Query message parameters or information about the message.
  HTuple GetMessageParam(const char* GenParamName, const char* Key) const;

  // Set message parameter or invoke commands on the message.
  void SetMessageParam(const HString& GenParamName, const HTuple& Key, const HTuple& GenParamValue) const;

  // Set message parameter or invoke commands on the message.
  void SetMessageParam(const HString& GenParamName, const HString& Key, const HTuple& GenParamValue) const;

  // Set message parameter or invoke commands on the message.
  void SetMessageParam(const char* GenParamName, const char* Key, const HTuple& GenParamValue) const;

  // Retrieve an object associated with the key from the message.
  HObject GetMessageObj(const HString& Key) const;

  // Retrieve an object associated with the key from the message.
  HObject GetMessageObj(const char* Key) const;

  // Add a key/object pair to the message.
  void SetMessageObj(const HObject& ObjectData, const HString& Key) const;

  // Add a key/object pair to the message.
  void SetMessageObj(const HObject& ObjectData, const char* Key) const;

  // Retrieve a tuple associated with the key from the message.
  HTuple GetMessageTuple(const HString& Key) const;

  // Retrieve a tuple associated with the key from the message.
  HTuple GetMessageTuple(const char* Key) const;

  // Add a key/tuple pair to the message.
  void SetMessageTuple(const HString& Key, const HTuple& TupleData) const;

  // Add a key/tuple pair to the message.
  void SetMessageTuple(const char* Key, const HTuple& TupleData) const;

  // Create a new empty message.
  void CreateMessage();

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HToolArrayRef;

typedef HToolArrayRef<HMessage> HMessageArrayRef;
typedef HSmartPtr< HMessageArrayRef > HMessageArrayPtr;


// Represents multiple tool instances
class LIntExport HMessageArray : public HToolArray
{

public:

  // Create empty array
  HMessageArray();

  // Create array from native array of tool instances
  HMessageArray(HMessage* classes, Hlong length);

  // Copy constructor
  HMessageArray(const HMessageArray &tool_array);

  // Destructor
  virtual ~HMessageArray();

  // Assignment operator
  HMessageArray &operator=(const HMessageArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HMessage* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HMessageArrayPtr *mArrayPtr;
};

}

#endif