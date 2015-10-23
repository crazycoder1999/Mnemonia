// Tabbed pane project template
#ifndef BlackSound_HPP_
#define BlackSound_HPP_

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <bb/cascades/WebView>
#include <bb/cascades/ListView>
#include <bb/cascades/Label>
#include <bb/cascades/Button>
//#include <QtCore/QObject>
#include <QtCore/QMetaType>

#include <bb/cascades/Event>
#include <bb/cascades/UiObject>
#include <bb/cascades/Control>
#include <bb/cascades/ListItemTypeMapper>
#include <bps/navigator.h>
#include "MySqlDataModel.h"
#include <bb/system/SystemUiResult>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemDialog>
#include <bb/system/SystemToast>
#include <bb/system/SystemProgressDialog>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/cascades/AbstractPane>
#include <bb/system/CardDoneMessage.hpp>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
#define REQUESTCODE 1
#define REQUESTTOKEN 2
#define GOTTOKEN 3
#define RETRIEVELIST 4
#define ADDREQUEST 5
#define POSTADD 10 //per i link che arrivano posticipati.
#define DELETEREQUEST 6
#define CHECKMARKREQUEST 7
#define FAVREQUEST 8
#define DOWNLOAD 9
#define DONE 0

class BlackSound : public QObject
{
    Q_OBJECT
private:
    // The central class to manage invocations
    bb::system::InvokeManager *m_invokeManager;
    QNetworkAccessManager *mNetworkAccessManager;
    bool netStatus;
    QString *appCode;
    QString *token;
    QString lastId;
    Button *btnToken;
    QObject *btnSync;
    QObject *logoutBtn;
    QObject *addItem;
    Label *labelProblem;
    QMap<QString,QString> dwnldList;
    QStringList *toAddList;
    int processedDownload;
    bb::system::SystemPrompt *prompt;
    bb::system::SystemProgressDialog *progressDialog;
    int state;
    /*settings...*/
    int blackTheme;
    int offlineMode;
    int orderBy;
    int limitArticle;
    /**/
    bb::cascades::WebView *myWebView;
    bb::cascades::ListView *myListView;
    bb::system::SystemToast *toastMsg;
    bool stillOpen;
    void nextDownload();
    void downloadList();
    bool loadTokens();
    void postAdd(QString);
    void nextLink();
    void saveTokens();
    void addRequest(QString);
   // MySqlDataModel *mSql;
    AbstractPane *root;
    void download(QString articleURL);
    void trashOfflineFiles();
    bool isFilePresent(QString);
    void saveOffline(QString,QString);
    void simpleAdd(QString);
    void loadLink();
    void loadSettings();
public:
    BlackSound(bb::cascades::Application *app);
    Q_INVOKABLE void askCode();
    Q_INVOKABLE void launchAuthBrowser();
    Q_INVOKABLE void askToken();
    Q_INVOKABLE void retrieveList();
    Q_INVOKABLE void askUrl();
    Q_INVOKABLE void deleteRequest(QString);
    Q_INVOKABLE void checkmarkRequest(QString);
    Q_INVOKABLE void favouriteRequest(QString);
    Q_INVOKABLE void toClipBoard(QString);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void launchBrowserTo(QString);
    Q_INVOKABLE void closeMe();
    Q_INVOKABLE void saveSettings(int,int,int,int);
    void processToAddLink();
    virtual ~BlackSound() { bps_shutdown(); if(toastMsg) delete(toastMsg); if(m_invokeManager) delete(m_invokeManager);}
signals:
	void mySignal(QString id,QString title,QString desc,QString lnk);
	void cleanList();
	void removeItem(QString id);
//	void shareSignal(QString lnk);
private slots:
    void requestFinished(QNetworkReply* reply);
    void onPromptFinished(bb::system::SystemUiResult::Type);
    void onLogoutFinished(bb::system::SystemUiResult::Type);
    void onProgressFinished(bb::system::SystemUiResult::Type);
 //   void networkStatusUpdateHandler(bool, QString);
public Q_SLOTS:
    // This method is invoked to notify the invocation system that the action has been done
    void cardDone();
private Q_SLOTS:
    // This slot is called whenever an invocation request is received
    void handleInvoke(const bb::system::InvokeRequest&);
};

#endif /* BlackSound_HPP_ */
