#ifndef CORE_SUPPORT_CORE_API_H
#define CORE_SUPPORT_CORE_API_H

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
#  if defined(CORE_DLL_EXPORT)
#    define CORE_API __declspec(dllexport)
#  elif defined(CORE_DLL_IMPORT)
#    define CORE_API __declspec(dllimport)
#  endif
#endif // defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)

#ifndef CORE_API
#  define CORE_API
#endif

#endif  // CORE_SUPPORT_CORE_API_H
