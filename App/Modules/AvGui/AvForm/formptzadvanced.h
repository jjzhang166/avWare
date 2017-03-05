#ifndef FORMPTZADVANCED_H
#define FORMPTZADVANCED_H

#include <QWidget>
#include <AvCapture/AvCapture.h>

namespace Ui {
class FormPtzAdvanced;
}

class FormPtzAdvanced : public QWidget
{
    Q_OBJECT

public:
    explicit FormPtzAdvanced(QWidget *parent = 0);
    ~FormPtzAdvanced();
	void	SetCurrentChannel(Capture *pCapture);

	void	FillInCaps();
	void	FillInProfile();

private slots:
          void on_BtnPtzASceneSubmit_clicked();
private:
    Ui::FormPtzAdvanced *ui;
	Capture			*m_Capture;
};

#endif // FORMPTZADVANCED_H
