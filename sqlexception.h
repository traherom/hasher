#ifndef SQLEXCEPTION_H
#define SQLEXCEPTION_H

#include <string>
using std::string;

class SQLException
{
public:
    SQLException() {}
    SQLException(string err) : mErr(err) { }
    SQLException(string err, string detail) : mErr(err), mDetail(detail) { }

    const string &error() const { return mErr; }
    const string &detail() const { return mDetail; }
    const string fullError() const { return mErr + ": " + mDetail; }

private:
    string mErr;
    string mDetail;
};

std::ostream &operator<<(std::ostream &o, const SQLException s);

#endif // SQLEXCEPTION_H
