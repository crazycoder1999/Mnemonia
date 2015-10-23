/*
 * MySqlDataModel.cpp
 *
 *  Created on: 29 feb 2012
 *      Author: ostenlund
 */
/**
 * select * from mylinks;
update mylinks set checked = 1 where id ="00002";
update mylinks set trash = 1 where id ="00002";
update mylinks set fav = 1 where id ="00002";
delete from mylinks where id ="00003";
insert into mylinks VALUES ("00001","http://www.google.it","google titolo","prova descrizione","img0001.png",0,0,0);
 * */

#include "MySqlDataModel.h"
#include "MyIndexMapper.h"
using namespace bb::cascades;
using namespace bb::data;

MySqlDataModel::MySqlDataModel() :
		mState(0), m_pSqlConnection(NULL), m_pSqlWorker(NULL) {

/*	QVariantMap map;
	map["loadingitem"] = true;

	modelList.append(map);*/
	if( !QFile::exists("data/mylinks.db") ) {
		QFile::copy("app/native/assets/sql/mylinks.db","data/mylinks.db");
	}

	connect(this, SIGNAL(loadSQLData(const QString&)), this,
			SLOT(onLoadData(const QString&)), Qt::QueuedConnection);
}

void MySqlDataModel::insertLink(QString id,QString title,QString desc,QString link) {
	qDebug() << "inserting link to db..." ;
	mState = ADD;
	title.replace("'","''");
	desc.replace("'","''");
//	insert into mylinks VALUES ("00001","http://www.google.it","google titolo","prova descrizione","img0001.png",0,0,0);
	QString query("insert into mylinks VALUES ('");
	query.append(id);
	query.append("','");
	query.append(link);
	query.append("','");
	query.append(title.toUtf8());
	query.append("','");
	query.append(desc.toUtf8());
	query.append("','NULL',0,0,0)");

	qDebug() << "QUERY " << query;
	addItem(id,title,desc,link);
	emit loadSQLData(query);
}

void MySqlDataModel::checkLink(QString id) {
	mState = CHECKMARK;
	QString query("update mylinks set checked = 1 where id ='");
	query.append(id);
	query.append("'");
	emit loadSQLData(query);
	removeItemWithId(id,false);
}

void MySqlDataModel::deleteById(QString id) {
	mState = DELETE;
	QString query("delete from mylinks where id = '");
	query.append(id);
	query.append("'");
	emit loadSQLData(query);
}

void MySqlDataModel::deleteAll() {
	mState = DELETE;
	QString query("delete from mylinks");
	emit loadSQLData(query);
}

void MySqlDataModel::loadData() {
	loadData("id ASC");
}

void MySqlDataModel::loadData(QString orderType) {
	mState = SYNC;
	QString query("select * from mylinks where checked is 0 order by ");
	query.append(orderType);
	emit loadSQLData(query);
	//emit loadSQLData("select * from mylinks where checked is 0 order by id ASC");
}

void MySqlDataModel::removeLoadingItem() {
	modelList.removeLast();
	QVariantList indexPath;
	indexPath.append(modelList.size());
	emit itemRemoved(indexPath);
}

MySqlDataModel::~MySqlDataModel() {

}

int MySqlDataModel::childCount(const QVariantList &indexPath) {
	if (indexPath.isEmpty())
		return modelList.size();
	else
		return 0;
}

bool MySqlDataModel::hasChildren(const QVariantList &indexPath) {
	if (indexPath.isEmpty())
		return !modelList.isEmpty();
	else
		return false;
}

QVariant MySqlDataModel::data(const QVariantList &indexPath) {
	return modelList.at(indexPath.back().toInt());
}

////	insert into mylinks VALUES ("00001","http://www.google.it","google titolo","prova descrizione","img0001.png",0,0,0);
void MySqlDataModel::addItem(QString id,QString title,QString desc,QString lnk) {

	//QMap<QString,QVariant> newRecord;
	QVariantMap newRecord;
	newRecord["ID"] = id;
	newRecord["DESC"] = desc;
	newRecord["LINK"] = QUrl::fromEncoded(lnk.toLocal8Bit(),QUrl::StrictMode);
	newRecord["TITLE"] = title;
	newRecord["THUMBNAIL"] = "NULL";
	newRecord["checked"] = qlonglong(0);
	newRecord["fav"] = qlonglong(0);
	newRecord["trash"] = qlonglong(0);

	int addIndex = modelList.size();
	modelList.append(newRecord);

	emit itemsChanged(DataModelChangeType::AddRemove,QSharedPointer<MyIndexMapper>(new MyIndexMapper(addIndex, 1, false)));
	emit initialItemsLoaded();
	//qDebug() << "arrivati: " << id << " [" << desc << "] " << title << " ["<< lnk << "]";

}

void MySqlDataModel::removeItemWithId(const QVariant &id,bool deleteFromDB) {
	QString idStr = id.toString();
	for(int i=0;i<modelList.size();i++){
		QString z = modelList.at(i).toMap()["ID"].toString();

		if(z.compare(idStr) == 0) {
			qDebug() << "found at index " << i;
			if(deleteFromDB)
				deleteById(idStr);
			modelList.removeAt(i);
			emit itemsChanged(DataModelChangeType::AddRemove,
						QSharedPointer<MyIndexMapper>(new MyIndexMapper(i, 1, true)));
			break;
		}
	}
	/*emit itemsChanged(DataModelChangeType::AddRemove,
				QSharedPointer<MyIndexMapper>(new MyIndexMapper(0, modelList.size(), true)));*/
}

void MySqlDataModel::removeItems() {

	int size = modelList.size();
	for (int i = 0; i < size; i++) {
		modelList.removeAt(0);
	}

	emit itemsChanged(DataModelChangeType::AddRemove,
			QSharedPointer<MyIndexMapper>(new MyIndexMapper(0, size, true)));
}

// Creates the SQL connection used for requesting SQL data and connects
// the SqlConnection::reply() signal to a custom clot used for handling the reply data.
void MySqlDataModel::onLoadData(const QString& sql) {
	if (!m_pSqlWorker) {
		m_pSqlWorker = new SqlWorker("./data/mylinks.db",
				this);
		m_pSqlConnection = new SqlConnection(m_pSqlWorker, this);
		bool res = connect(m_pSqlConnection,
				SIGNAL(reply(const bb::data::DataAccessReply&)), this,
				SLOT(onReply(const bb::data::DataAccessReply&)));

		Q_ASSERT(res);

		// Indicate that the variable success isn't used in the rest of the app, to
		// prevent a compiler warning
		Q_UNUSED(res);
	}

	m_pSqlConnection->execute(sql);
}

void MySqlDataModel::appendNewList(QVariantList resultList){
	/*for(int i=0;i<modelList.count();i++) {
		qDebug() << "COUNT SUB " << modelList.at(i).toMap().count();
		for(int j=0;j<modelList.at(i).toMap().keys().count();j++) {
			QString z = modelList.at(i).toMap().keys()[j];
			qDebug() << "Key: " << z << " Value: " << modelList.at(i).toMap()[z];
		}
	}*/
	int addIndex;

	if(resultList.size() == 0)
		return;

	addIndex = modelList.size();
	modelList.append(resultList);

	emit itemsChanged(DataModelChangeType::AddRemove,
			QSharedPointer<MyIndexMapper>(
					new MyIndexMapper(addIndex, resultList.size(), false)));
	emit initialItemsLoaded();
}

// Receives the data returned by the asynchronous
// SQL request.
void MySqlDataModel::onReply(const bb::data::DataAccessReply& dar) {

	if(mState == SYNC){
		qDebug() << "sync arrived...";
		QVariantList resultList = dar.result().value<QVariantList>();
		//addItem("aa","aaa","abbabab","oajjoa");
		appendNewList(resultList);
	}
	if(mState == DELETE) {
		qDebug() << "delete arrived";
	}
	if(mState == ADD ) {
		qDebug() << "add .. done!";
	}
	if(mState == CHECKMARK) {
		qDebug() << "checkmark... done!";
	}
	mState = NOOP;
}
