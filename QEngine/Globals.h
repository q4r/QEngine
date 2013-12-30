#define SAFEDELETE(pnt) if (pnt) {delete pnt; pnt = 0;}
#define SAFERELEASE(pnt) if (pnt) {pnt->Release(); pnt = 0;}
#define LOG(x) OutputDebugStringA(x);
//#define LOG(x) std::cout << x << std::endl

#include <string>
#include <vector>
typedef std::vector<std::string> Strings;

void Split(std::string data, char c, Strings& strings);