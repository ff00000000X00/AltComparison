#include <QJsonObject>


class JSONWriter{
public:
    bool whrite(const QString &path = {});
    void setJsObject(const QJsonObject & js);
private:
    QJsonObject m_obj;
};