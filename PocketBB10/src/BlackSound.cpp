// Tabbed pane project template
#include "BlackSound.hpp"
#include "HtmlRemoval.h"
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/SceneCover>
#include <bb/data/JsonDataAccess>
//#include <QIODevice>
//#include "MySqlDataModel.h"
#include <bb/system/Clipboard>
#include <bb/system/InvokeManager>
#include <bps/bps.h>
#include <bps/netstatus.h>
#include "ActiveFrame.h"

using namespace bb::cascades;
using namespace bb::data;
using namespace bb::system;
using namespace cesare;

#define STOPDOWNLOAD 10000
BlackSound::BlackSound(bb::cascades::Application *app)
: QObject(app)   , m_invokeManager(new InvokeManager(this)) , toAddList(NULL) ,progressDialog(NULL),toastMsg(NULL) , root(NULL)
{

    connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
	state = REQUESTCODE;
	bool result;
	QmlDocument *qml;
    switch(m_invokeManager->startupMode()) {
    case ApplicationStartupMode::LaunchApplication:
    		bps_initialize();
    		mNetworkAccessManager = new QNetworkAccessManager(this);
    		toAddList = new QStringList();
    		result = connect(mNetworkAccessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
    		Q_ASSERT(result);
    		Q_UNUSED(result);
    		stillOpen = true;
        break;
    case ApplicationStartupMode::InvokeApplication:
    		qml = QmlDocument::create("asset:///invoked.qml").parent(this);
    		qml->setContextProperty("app", this);
    		root = qml->createRootObject<AbstractPane>();
    		app->setScene(root);
    		stillOpen = false;
    		//bps_shutdown();
    		return;
    default:
        // What app is it and how did it get here?
        break;
    }

	qmlRegisterType<MySqlDataModel>("custom",1,0,"MySqlDataModel");
	//mSql = new MySqlDataModel();
	qml = QmlDocument::create("asset:///viewrecord.qml").parent(this);
    qml->setContextProperty("app", this);

    QDeclarativePropertyMap* settingsMap = new QDeclarativePropertyMap;
    loadSettings();
    settingsMap->insert("isblack", QVariant(blackTheme?true:false));
    settingsMap->insert("offlinemode", QVariant(offlineMode?true:false));
    settingsMap->insert("limitarticle",QVariant(limitArticle));
    settingsMap->insert("orderby",QVariant(orderBy));
    qml->setContextProperty("settingsmap", settingsMap);

    QString workingDir = QDir::currentPath();

    // Build the path, add it as a context property, and expose it to QML
    QDeclarativePropertyMap* dirPaths = new QDeclarativePropertyMap;
    dirPaths->insert("home", QVariant(QString(
            "file://" + workingDir)));
    qml->setContextProperty("dirPaths", dirPaths);

    //prompt = new SystemPrompt();
    root = qml->createRootObject<AbstractPane>();
    myListView = root->findChild<ListView*>("myListView");
    app->setScene(root);
    btnToken = root->findChild<Button*>("btnToken");
    btnSync = root->findChild<QObject*>("syncBtn");
    logoutBtn = root->findChild<QObject*>("logoutBtn");
    addItem = root->findChild<QObject*>("addItem");
    labelProblem = root->findChild<Label*>("labelProblem");
    if(loadTokens()) {
    	Application::instance()->setCover(new ActiveFrame());
    	myListView->setProperty("visible",true);
    	root->findChild<Button*>("btnCode")->setProperty("visible",false);
    	btnToken->setProperty("visible",false);
    	logoutBtn->setProperty("enabled",true);
    	btnSync->setProperty("enabled",true);
    	addItem->setProperty("enabled",true);
    	state = DONE;
    }


}

void BlackSound::closeMe() {
	cardDone();
//	exit( EXIT_SUCCESS );
	Application::exit(EXIT_SUCCESS);
}
//! [0]

/*void BlackSound::networkStatusUpdateHandler(bool status, QString type) {
	netStatus = status;
	qDebug() << "NETSTATUS: " << netStatus;
}*/

void BlackSound::cardDone()
{
    CardDoneMessage message;
    message.setData(tr("Card: I am done. yay!"));
    message.setDataType("text/plain");
    message.setReason(tr("Success!"));

    m_invokeManager->sendCardDone(message);
}

void BlackSound::loadLink() {
    FILE *f;
    char z[500];
    if(toAddList) {
    	toAddList->clear();
    	delete(toAddList);
	}
    toAddList = new QStringList();
    f = fopen("data/todownload","r+");
    if(f==NULL)
    	return;
    while(!feof(f)) {
    	fscanf(f,"%s",z);
    	if(strstr(z,"http")!=NULL)
    		toAddList->append(z);
    	strcpy(z,"/0");
    }
    fclose(f);
}

void BlackSound::processToAddLink() {
	if(toAddList==NULL) {
		state = DONE;
		return;
	}
    if( ! toAddList->empty()) { //I have some link to add to pocket.
    	processedDownload = 0;
    	nextLink();
    } else { // TODO dovrei scaricare gli articoli ma per ora aspetto... faccio le due cose con 2 sync separati...
    }
}

void BlackSound::nextLink() {

	if(processedDownload < toAddList->size()) {
		postAdd(toAddList->at(processedDownload++));
	} else {
		if(toAddList) {
			toAddList->clear();
			delete(toAddList);
		}
        QFile::remove("data/todownload");
        state = DONE;
	}
}

//! [1]
void BlackSound::handleInvoke(const InvokeRequest& request)
{

	qDebug("INVOKED!!!!!!!");
    // Copy data from incoming invocation request to properties
    QString m_source = QString::fromLatin1("%1 (%2)").arg(request.source().installId()).arg(request.source().groupId());
    QString m_target = request.target();
    QString m_action = request.action();
    QString m_mimeType = request.mimeType();
    QString m_uri = request.uri().toString();
    QString m_data = QString::fromUtf8(request.data());

    qDebug() << m_source << " " << m_target << " " << m_action << " " << m_mimeType << " " << m_uri << " " << m_data;

    if(stillOpen) { // TODO presumo sia connesso a internet.
    	addRequest(m_uri);
    } else { //link to download...
    	FILE *f;
    	f = fopen("data/todownload","a+");
    	fseek(f,0,SEEK_END);
    	fprintf(f,"%s",m_uri.toStdString().c_str());
    	fprintf(f,"\n");
    	fclose(f);
    }

    // Signal that the properties have changed
 //   emit requestChanged();
}

void BlackSound::askToken() {
	QNetworkRequest request = QNetworkRequest();
	request.setUrl(QUrl("https://getpocket.com/v3/oauth/authorize?"));
	state = REQUESTTOKEN;
	QString qStr;
	qStr.append("{\"consumer_key\":\"12345678901234567890\",\"code\":\"");//CHANGE HERE
	qStr.append(appCode);
	qStr.append("\"}");
	qDebug() << qStr.toUtf8();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "Content-Type: application/json; charset=UTF-8");
	request.setRawHeader("X-Accept:","application/json"); //X-Accept: application/json
	mNetworkAccessManager->post(request, qStr.toUtf8());
	progressDialog = new SystemProgressDialog(NULL);
    progressDialog->setTitle("Finalizing");
    progressDialog->setBody("... almost done!");
    progressDialog->defaultButton()->setLabel("Please Wait :)");
	bool success = QObject::connect(progressDialog,SIGNAL(finished(bb::system::SystemUiResult::Type)),this,
	         SLOT(onProgressFinished(bb::system::SystemUiResult::Type)));
    if (success) {
        progressDialog->show();
  //      state=DONE;
    } else {
    	state=DONE;
        progressDialog->deleteLater();
    }
}

void BlackSound::download(QString articleURL) {
	state = DOWNLOAD;
	qDebug() << "Downloading " << articleURL;
	QNetworkRequest request = QNetworkRequest(QUrl(articleURL));
	mNetworkAccessManager->get(request);
}

void BlackSound::postAdd(QString url) {
	state = POSTADD;
	simpleAdd(url);
}

void BlackSound::addRequest(QString url) {
	state = ADDREQUEST;
	simpleAdd(url);
}

void BlackSound::simpleAdd(QString url) {
	QUrl theUrl(url);
	QString encodedUrl = theUrl.toEncoded();
	QNetworkRequest request = QNetworkRequest();
	request.setUrl(QUrl("https://getpocket.com/v3/add?"));
	QString qStr;
	qStr.append("{\"url\":\"");
	qStr.append(encodedUrl);
	qStr.append("\",\"consumer_key\":\"12345678901234567890\",\"access_token\":\""); //CHANGE HERE
	qStr.append(token);
	qStr.append("\"}");
	qDebug() << qStr.toUtf8();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "Content-Type: application/json; charset=UTF-8");
	request.setRawHeader("X-Accept:","application/json"); //X-Accept: application/json
	mNetworkAccessManager->post(request, qStr.toUtf8());
}

void BlackSound::deleteRequest(QString id) {
	state = DELETEREQUEST;
	lastId = id;
	QNetworkRequest request = QNetworkRequest();
	QString qStr;
	qStr.append("https://getpocket.com/v3/send?");
	QString qParam;
	qParam.append("actions=[{\"action\":\"delete\",\"item_id\":\"");
	qParam.append(id);
	qParam.append("\"}]");
	qParam.append("&access_token=");
	qParam.append(token);
	qParam.append("&consumer_key=12345678901234567890"); //CHANGE HERE
	QUrl z(qStr);
	request.setUrl(qStr);
	qDebug() << "NEW URL DELETE "<< qParam;
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF8");
	request.setRawHeader("X-Accept:","application/json"); //X-Accept: application/json
	mNetworkAccessManager->post(request, qParam.toUtf8());
}

void BlackSound::favouriteRequest(QString id) {
	state = FAVREQUEST;
	lastId = id;
	QNetworkRequest request = QNetworkRequest();
	QString qStr;
	qStr.append("https://getpocket.com/v3/send?");
	QString qParam;
	qParam.append("actions=[{\"action\":\"favorite\",\"item_id\":\"");
	qParam.append(id);
	qParam.append("\"}]");
	qParam.append("&access_token=");
	qParam.append(token);
	qParam.append("&consumer_key=12345678901234567890"); //CHANGE HERE
	QUrl z(qStr);
	request.setUrl(qStr);
	qDebug() << "NEW URL Favorite "<< qParam;
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF8");
	request.setRawHeader("X-Accept:","application/json"); //X-Accept: application/json
	mNetworkAccessManager->post(request, qParam.toUtf8());
}

void BlackSound::checkmarkRequest(QString id) {
	state = CHECKMARKREQUEST;
	lastId = id;
	QNetworkRequest request = QNetworkRequest();
	QString qStr;
	qStr.append("https://getpocket.com/v3/send?");
	QString qParam;
	qParam.append("actions=[{\"action\":\"archive\",\"item_id\":\"");
	qParam.append(id);
	qParam.append("\"}]");
	qParam.append("&access_token=");
	qParam.append(token);
	qParam.append("&consumer_key=12345678901234567890"); //CHANGE HERE
	QUrl z(qStr);
	request.setUrl(qStr);
	qDebug() << "NEW URL CHECKED "<< qParam;
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF8");
	request.setRawHeader("X-Accept:","application/json"); //X-Accept: application/json
	mNetworkAccessManager->post(request, qParam.toUtf8());
}

void BlackSound::retrieveList() {
	qDebug() << "STATE " << state;
	if(state != DONE)
		return;
	state = RETRIEVELIST;
	QNetworkRequest request = QNetworkRequest();
	request.setUrl(QUrl("https://getpocket.com/v3/get?"));
	QString qStr;
	qStr.append("{\"consumer_key\":\"12345678901234567890\",\"access_token\":\""); //CHANGE HERE
	qStr.append(token);
	qStr.append("\"}");
	request.setHeader(QNetworkRequest::ContentTypeHeader, "Content-Type: application/json; charset=UTF-8");
	request.setRawHeader("X-Accept:","application/json"); //X-Accept: application/json
	mNetworkAccessManager->post(request, qStr.toUtf8());
}

void BlackSound::askCode() {
	QNetworkRequest request = QNetworkRequest();
	request.setUrl(QUrl("https://getpocket.com/v3/oauth/request?"));
	QString qStr;
	qStr.append("{\"consumer_key\":\"12345678901234567890\",\"redirect_uri\":\"http://www.yourwebpagetoconfirmisok.com/ok.html\"}"); //CHANGE HERE! 2 PARAMS
	qDebug() << qStr.toUtf8();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "Content-Type: application/json; charset=UTF-8");
	request.setRawHeader("X-Accept:","application/json"); //X-Accept: application/json
	mNetworkAccessManager->post(request, qStr.toUtf8());
}

void BlackSound::launchBrowserTo(QString url) {
	navigator_invoke(url.toStdString().c_str(), NULL);
}

void BlackSound::launchAuthBrowser() {
	QString authUrl;
	authUrl.append("https://getpocket.com/auth/authorize?request_token=");
	authUrl.append(appCode);
	authUrl.append("&redirect_uri=http://www.yourwebpagetoconfirmisok.com/ok.html"); //CHANGE HERE!
	qDebug() << "[" << authUrl.toStdString().c_str() << "]";
	navigator_invoke(authUrl.toStdString().c_str(), NULL);
}

bool BlackSound::loadTokens() {
	if(QFile::exists("data/tokens")) {
		qDebug()<<"start reading..";
		QFile file("data/tokens");
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QTextStream in(&file);
		token = new QString(in.readLine());
		appCode = new QString("");

		//token->append(t);
		file.close();
		qDebug()<<"end reading.." << token->toStdString().c_str();
		return true;
	}
	return false;
}

void BlackSound::nextDownload() {
	processedDownload++;
	QList<QString> keys = dwnldList.keys();
	if(processedDownload >= keys.length()) {
		dwnldList.clear();
		progressDialog->deleteLater();
		state = DONE;
		return;
	}

	download(dwnldList[keys[processedDownload]]);
}

void BlackSound::downloadList() {
	if(offlineMode==0) {
		state = DONE;
		return;
	}
	progressDialog = new SystemProgressDialog(NULL);
    progressDialog->setTitle("Syncing");
    progressDialog->setBody("I'm downloading your articles...please wait :)");
    progressDialog->defaultButton()->setLabel("Skip");
	bool success = QObject::connect(progressDialog,SIGNAL(finished(bb::system::SystemUiResult::Type)),this,
	         SLOT(onProgressFinished(bb::system::SystemUiResult::Type)));
    if (success) {
        progressDialog->show();
    } else {
        // Failed to connect to signal.
        progressDialog->deleteLater();
    }

	processedDownload = 0;
	QList<QString> keys = dwnldList.keys();
	if(keys.length() == 0) {
		state = DONE;
		return;
	}
	download(dwnldList[keys[0]]);
}

void BlackSound::saveTokens() {
	FILE *f;
	f = fopen("data/tokens","w");
	fprintf(f,"%s",token->toStdString().c_str());
	fclose(f);
}

void BlackSound::requestFinished(QNetworkReply* reply)
{
    // Check the network reply for errors

    if (reply->error() == QNetworkReply::NoError) {

    	QByteArray bArray = reply->readAll();
    	QString strReply = QString::fromUtf8(bArray);
    	qDebug() << "arrived " << strReply << "st: "<< state;
    	if(state==REQUESTCODE) {
    		appCode = new QString(strReply.mid(5));
    		launchAuthBrowser();
    		btnToken->setProperty("visible",true);
    		btnToken->setProperty("enabled",true);
    	} else if(state==REQUESTTOKEN) {
    		qDebug() << "request token: " << strReply;

    			int fine = strReply.indexOf("&",0);
    			int inizio = strReply.indexOf("=",0);
    			token = new QString(strReply.mid(inizio+1,fine-inizio-1));
    			QString *username = new QString(strReply.mid(strReply.lastIndexOf("=")+1));
    			qDebug() << "username: " << *username;
    			saveTokens();
    			loadTokens();
    			progressDialog->deleteLater();
    			state=DONE;

    	} else if(state==RETRIEVELIST) {
    		qDebug() << "list! " << strReply;
    		emit cleanList();
    		JsonDataAccess jda;
    		QVariant resp = jda.loadFromBuffer(bArray);
    		qDebug() << "error json " << jda.error().errorMessage();
    		QMap<QString,QVariant> articles = resp.toMap()["list"].toMap();
    		int nArticles = articles.count();
    		QList<QString> keys = articles.keys();
    		trashOfflineFiles(); //da rivedere...
    		dwnldList.clear();
    		for(int i=0;i<nArticles;i++) {
    			//qDebug() << "key: " << keys[i];
    			qDebug() << "READ "<< articles[keys[i]].toMap()["resolved_title"] << " URL " << articles[keys[i]].toMap()["resolved_url"];
    			QString itemId = articles[keys[i]].toMap()["item_id"].toString();
    			QString link = articles[keys[i]].toMap()["resolved_url"].toString().length()==0?articles[keys[i]].toMap()["normal_url"].toString():articles[keys[i]].toMap()["resolved_url"].toString(); //normal_url???

        		QString title = articles[keys[i]].toMap()["resolved_title"].toString();
        		QString shortDesc = articles[keys[i]].toMap()["excerpt"].toString();
        		emit mySignal(itemId,title.length()==0?link:title,shortDesc.length()==0?"-":shortDesc,link);
        		//emit mySignal(itemId,"TITLE","DESC",link);
    			dwnldList[itemId] = link;
    		}
    		loadLink(); //load from file the list of link...
    		qDebug()<<"ADD LIST " << toAddList->size() << " empty? " << toAddList->empty();
    		for(int i = 0;i<toAddList->size();i++)
    			qDebug() << "[" << toAddList->at(i) << "]";
    		if(toAddList->empty()) {//none link to add
    			downloadList(); //download the articles
    		} else { // else I add pending link.
    			qDebug() << "link to add";
    			processToAddLink();
    		}
    	} else if( state == ADDREQUEST || state == POSTADD ) {
    		JsonDataAccess jda;
    		QVariant respJSON = jda.loadFromBuffer(bArray);
    		QVariant resp = respJSON.toMap()["item"];
    		QString link = resp.toMap()["resolved_url"].toString().length()==0?resp.toMap()["normal_url"].toString():resp.toMap()["resolved_url"].toString(); //normal_url???
    		QString title = resp.toMap()["title"].toString();
    		QString shortDesc = resp.toMap()["excerpt"].toString();
    		//qDebug() << "QUIII ADD " << link << " " << title << " [] " << shortDesc;
    		qDebug() << "Normal URL " << resp.toMap()["normal_url"].toString() << " RESOLVED "<< resp.toMap()["resolved_url"].toString();
    		emit mySignal(resp.toMap()["item_id"].toString(),title.length()==0?link:title,shortDesc.length()==0?"-":shortDesc,link);
    		if(state == ADDREQUEST) {
    	   		processedDownload = 0;
    			dwnldList.clear();
    			dwnldList[resp.toMap()["item_id"].toString()] =link;
    			downloadList();
    		} else if(state == POSTADD){
    			nextLink();
    		}
    	} else if(state==DELETEREQUEST) {
    		JsonDataAccess jda;
    		QVariant respJSON = jda.loadFromBuffer(bArray);
    		emit removeItem(lastId);
    		state = DONE;
    		qDebug() << "DELETED DONE, status: " << respJSON.toMap()["status"].toInt();
    	} else if(state == FAVREQUEST){
    		state = DONE;
    	}else if(state==CHECKMARKREQUEST) {
    		JsonDataAccess jda;
    		QVariant respJSON = jda.loadFromBuffer(bArray);
    		emit removeItem(lastId);
    		state = DONE;
    		qDebug() << "ARCHIVED DONE, status: " << respJSON.toMap()["status"].toInt();
    	} else if(state == DOWNLOAD) {
    		int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();
    		qDebug() << "DOWNLOAD!!! " << httpstatuscode;
    		if(processedDownload>=STOPDOWNLOAD) {
    			qDebug() << "...esco prima...";
    			state = DONE;
    			return;
    		}

    		QString prgStr;
    		prgStr.append("I'm downloading your articles ");
    		prgStr.append(QString::number(processedDownload+1));
    		prgStr.append(" / ");
    		prgStr.append(QString::number(dwnldList.keys().size()));
    		prgStr.append(" .. please wait :)");
    	    progressDialog->setBody(prgStr);
    	    progressDialog->show();
    		switch(httpstatuscode) {
    			case 200:
    					if (reply->isReadable()) {
    						if(processedDownload>=STOPDOWNLOAD){
    							progressDialog->deleteLater();
    							state = DONE;
    							break;
    						} else {
    							saveOffline(dwnldList.keys()[processedDownload],strReply);
    						}
    					nextDownload();
    				}
    				break;
    			default:
    					nextDownload();
    					break;
    		}
    	}
    } else {
    	if(state == DOWNLOAD) {
    		nextDownload();
    	}
    	if(state == RETRIEVELIST || state == CHECKMARKREQUEST || state == FAVREQUEST || state == DELETEREQUEST) {
    		SystemToast *toast = new SystemToast(this);
    		toast->setBody("Connection problem ...");
    		toast->setPosition(SystemUiPosition::MiddleCenter);
    		toast->show();
    		state = DONE;
    	}
    	if(state == REQUESTTOKEN ) {
    		int forbiddenAccess = reply->errorString().indexOf("Forbidden");
    		if( forbiddenAccess != -1 ) {
    		    qDebug() << "Forbidden!";
    		    btnToken->setProperty("visible",false);
    		    logoutBtn->setProperty("enabled",false);
    		    btnSync->setProperty("enabled",false);
    		    addItem->setProperty("enabled",false);
    		    labelProblem->setProperty("visible",true);
    		    progressDialog->deleteLater();
    		    myListView->setProperty("visible",false);
    		    state = DONE;
    		}
    	}
        qDebug() << "\n Problem with the network ...";
        qDebug() << "\n reply " << reply->errorString();
    }
}


void BlackSound::saveOffline(QString fname,QString content) {
	fname = "data/offline/" + fname;
	qDebug() << "FNAME: "+fname;
	if( isFilePresent(fname)){
		qDebug() << "SaveOffline already present: " << fname;
		return;
	}

	FILE *f;
	f = fopen(fname.toStdString().c_str(),"w+");
	fprintf(f,"%s",content.toStdString().c_str());
	fclose(f);

	HtmlRemoval htmlRemove;
	htmlRemove.elaborate(fname.toStdString());

}

void BlackSound::saveSettings(int blackT,int offlineM,int order,int articleLimit) {
	FILE *f;
	f = fopen("data/settingsnew","w");
	fprintf(f,"1 %d %d %d %d",blackT,offlineM,order,articleLimit);
	fclose(f);
}

void BlackSound::loadSettings( ){
	FILE *f;
	f=fopen("data/settingsnew","r");
	if(f!=NULL) {
		int settingsversion;
		fscanf(f,"%d %d %d %d %d",&settingsversion,&blackTheme,&offlineMode,&orderBy,&limitArticle);
		fclose(f);
	} else {
		blackTheme = 0; //disable by default
		offlineMode = 1; //enable by default
		limitArticle = -1;
		orderBy = 0;
	}
}

bool BlackSound::isFilePresent(QString fname) {
	QDir dir;
	dir.mkpath("data/offline");

	/*if (!dir.exists()) {
	    dir.mkpath("data/offline");
	}*/
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Size | QDir::Reversed);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
	    qDebug()<< qPrintable(QString("%1").arg(fileInfo.fileName()));
	    if( fileInfo.fileName().toStdString().compare(fname.toStdString()) == 0) {
	    	return true;
	    }
	}

	return false;
}

void BlackSound::trashOfflineFiles() {
	QDir dir("data/offline");

	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Size | QDir::Reversed);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
	    qDebug()<< "Cancello: "<< qPrintable(QString("%1").arg(fileInfo.fileName()));
	    QFile::remove("data/offline/"+fileInfo.fileName());
	}
}

void BlackSound::logout() {
	if(!QFile::exists("data/tokens"))
		return;

    SystemDialog *dialog = new SystemDialog("Yes, Logout!","No :)");
    dialog->setTitle("Logout");
    dialog->setBody("Are you sure you want to logout from getpocket.com?");
	bool success = QObject::connect(dialog,SIGNAL(finished(bb::system::SystemUiResult::Type)),this,
	         SLOT(onLogoutFinished(bb::system::SystemUiResult::Type)));

    if (success) {
        dialog->show();
    } else {
        dialog->deleteLater();
    }
}

void BlackSound::askUrl() {

	prompt = new SystemPrompt();
    prompt->setTitle("You can paste here the link");
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setEmptyText("Please fill the link, here..");

    bool success = QObject::connect(prompt,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinished(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
    } else {
        // Failed to connect to signal.
        prompt->deleteLater();
    }
}

void BlackSound::toClipBoard(QString linkToPaste) {
	bb::system::Clipboard clipboard;
	clipboard.clear();
	clipboard.insert("text/plain", linkToPaste.toStdString().c_str());
}

void BlackSound::onProgressFinished(SystemUiResult::Type type) {
	qDebug() << "skipped";
	processedDownload = STOPDOWNLOAD;
}

void BlackSound::onLogoutFinished(SystemUiResult::Type type) {
	qDebug()<<" logout...response";
    if (type != SystemUiResult::ConfirmButtonSelection) {
        qDebug() << "Logout Rejected:";
    } else {
        qDebug() << "Logout Accepted";
        QFile::remove("data/tokens");
        QFile::remove("data/mylinks.db");
    	myListView->setProperty("visible",false);
    	root->findChild<Button*>("btnCode")->setProperty("visible",true);
    	//root->findChild<Button*>("btnBrowser")->setProperty("visible",false);
    	root->findChild<Button*>("btnToken")->setProperty("visible",true);
    	root->findChild<Button*>("logoutBtn")->setProperty("visible",false);
    	root->findChild<Button*>("syncBtn")->setProperty("visible",false);
    	root->findChild<QObject*>("addItem")->setProperty("enabled",false);
    }
}

void BlackSound::onPromptFinished(SystemUiResult::Type type) {
    if (type != SystemUiResult::ConfirmButtonSelection) {
        qDebug() << "Prompt Rejected:";
    } else {
        qDebug() << "Prompt Accepted";
        addRequest(prompt->inputFieldTextEntry());
    }
    prompt->deleteLater();

}

