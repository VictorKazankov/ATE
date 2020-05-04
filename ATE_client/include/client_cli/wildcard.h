#ifndef ATE_CLIENT_CLIENT_CLI_WILDCARD_H_
#define ATE_CLIENT_CLIENT_CLI_WILDCARD_H_

#include "enums.h"
#include "errors.h"
#include "object.h"

namespace ATE {

// clang-format off

/** <summary>
      Class for wildcard matching of objects in Icon database. Wildcard can be used either to
      fetch object data from database or as an API argument (e.g. WaitForObject()).
    </summary>
**/
public ref class Wildcard {
 public:
  /** <summary>
        Get match result.
      </summary>
      <returns>
        The non-empty list of the ATE.Object if pattern matches to any object
        in the database, otherwise empty list.
      </returns>
      <exception cref="ATE::NoConnectionEstablished">Thrown if no connection to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::Exception">Thrown if internal error has occured.</exception>
  **/
  System::Collections::Generic::List<ATE::Object^>^ GetMatchObjects();

  /// <summary>SYNC product name pattern.</summary>
  property System::String^ SyncVersion {
    System::String^ get() { return sync_version_; }
    void set(System::String^ value) { sync_version_ = value; }
  }

  /// <summary>SYNC build revision pattern.</summary>
  property System::String^ SyncBuildVersion {
    System::String^ get() { return build_version_; }
    void set(System::String^ value) { build_version_ = value; }
  }

  /// <summary>HMI light mode pattern.</summary>
  property CollectionMode SyncCollectionMode {
    CollectionMode get() { return mode_; }
    void set(CollectionMode value) { mode_ = value; }
  }

  /// <summary>Object name pattern.</summary>
  property System::String^ Name {
    System::String^ get() { return name_; }
    void set(System::String^ value) { name_ = value; }
  }

  /// <summary>Parent screen name pattern.</summary>
  property System::String^ ParentScreen {
    System::String^ get() { return parent_screen_; }
    void set(System::String^ value) { parent_screen_ = value; }
  }

 private:
  System::String^ sync_version_{gcnew System::String("")};
  System::String^ build_version_{gcnew System::String("")};
  CollectionMode mode_{CollectionMode::None};
  System::String^ name_{gcnew System::String("")};
  System::String^ parent_screen_{gcnew System::String("")};
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_WILDCARD_H_
