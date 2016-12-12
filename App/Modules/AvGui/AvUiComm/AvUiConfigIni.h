#ifndef _AVUICONIFGINI_H_
#define _AVUICONIFGINI_H_


#include <QSettings>
#include <QString>
#include <QVariant>

class CAvUiConfigIni
{
public:
	CAvUiConfigIni();
	~CAvUiConfigIni();

public:
	static CAvUiConfigIni* Instance();
	void InisetValue(QString section, QString key, const QVariant &value);
	QVariant InigetValue(QString section, QString key);


	void InisetArrayValue(int index, QString &key, const QVariant &value);
	QVariant InigetArrayValue(int index, QString &key);
	int InibeginReadArray(const QString &prefix);
	void InibeginWriteArray(const QString &prefix, int size = -1);
	void IniendArray();
	
private:
	std::string			m_ConfigPath;
	QSettings		*	m_QSettings;
};









#endif

