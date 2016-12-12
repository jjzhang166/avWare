#include "AvUiConfigIni.h"
#include "AvGui/AvGui.h"
#include <QMutex>
static QMutex CAvUiConfigIni_mutex;
CAvUiConfigIni::CAvUiConfigIni()
{
	//QMutexLocker locker(&CAvUiConfigIni_mutex);
	m_ConfigPath = CAvGui::GetConfigsPath();
	m_ConfigPath += "/";
	m_ConfigPath += "AvGui.ini";
	QString file(m_ConfigPath.c_str());
	m_QSettings = new QSettings(file, QSettings::IniFormat, NULL);

}

CAvUiConfigIni::~CAvUiConfigIni()
{
	
}

void CAvUiConfigIni::InisetValue(QString section, QString key, const QVariant &value)
{
	//QMutexLocker locker(&CAvUiConfigIni_mutex);
	m_QSettings->setValue(section + "/" + key, value);
	m_QSettings->sync();
}
QVariant CAvUiConfigIni::InigetValue(QString section, QString key)
{
	//QMutexLocker locker(&CAvUiConfigIni_mutex);
	return m_QSettings->value(section + "/" + key);
}

void CAvUiConfigIni::InisetArrayValue(int index, QString &key, const QVariant &value)
{
	m_QSettings->setArrayIndex(index);
	m_QSettings->setValue(key, value);
	m_QSettings->sync();
}
QVariant CAvUiConfigIni::InigetArrayValue(int index, QString &key)
{
	m_QSettings->setArrayIndex(index);
	return m_QSettings->value(key);
}

int CAvUiConfigIni::InibeginReadArray(const QString &prefix)
{
	return m_QSettings->beginReadArray(prefix);
}
void CAvUiConfigIni::InibeginWriteArray(const QString &prefix, int size)
{
	m_QSettings->beginWriteArray(prefix, size);
}
void CAvUiConfigIni::IniendArray()
{
	m_QSettings->endArray();
}

CAvUiConfigIni*  CAvUiConfigIni::Instance()
{
	static CAvUiConfigIni* _instance = NULL;
	if (!_instance) {
		QMutexLocker locker(&CAvUiConfigIni_mutex);
		if (!_instance) {
			_instance = new CAvUiConfigIni;
		}
	}
	return _instance;
}
