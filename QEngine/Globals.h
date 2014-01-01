#include <fstream>
extern std::ofstream _log_file_;
#define START_LOGING if ( ! _log_file_.is_open() ) {_log_file_.open("LOG.txt");}
#define LOG_(x) _log_file_ << x
#define LOG(x) _log_file_ << x << std::endl
#define STOP_LOGING if ( _log_file_.is_open() ) {_log_file_.close();}


#define SAFEDELETE(pnt) if (pnt) {delete pnt; pnt = 0;}
#define SAFEDELETEARRAY(pnt) if (pnt) {delete[] pnt; pnt = 0;}
#define SAFERELEASE(pnt) if (pnt) {pnt->Release(); pnt = 0;}
//#define LOG(x) OutputDebugStringA(x);


#include <string>
#include <vector>
typedef std::vector<std::string> Strings;

void Split(std::string data, char c, Strings& strings);