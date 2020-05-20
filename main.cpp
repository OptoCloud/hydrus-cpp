#include <QApplication>
#include <QSet>

//#include "hydrusthumbnailscene.h"
#include "hydrusitem.h"

#define DEFAULT_WIDTH 150
#define DEFAULT_HEIGHT 125
#define DEFAULT_SPACING 5
int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	QApplication::setDesktopFileName("Hydrus");
	QApplication::setApplicationName("Hydrus");
	QApplication::setApplicationVersion("1.0");

	std::vector<HydrusID> ids;
	ids.push_back(HydrusID("oienbokkobnfebgmgneeaago"));
	ids.push_back(HydrusID("jcimhbindefdocmblnnlbgim"));
	ids.push_back(HydrusID("llpnbfbndmepjaeeeofmalfe"));
	ids.push_back(HydrusID("klepbekiijcipeaekjhlehhk"));
	ids.push_back(HydrusID("nmdkidblgnjodfdjakhkfode"));
	ids.push_back(HydrusID("dbhdpmfcehbkflpipfioakpk"));
	ids.push_back(HydrusID("boigfacholelabppiglldgfo"));
	ids.push_back(HydrusID("jimmonbpoabbmkdpbacghhbe"));
	ids.push_back(HydrusID("mopbkbpoldpjhbpkclimhkkk"));
	ids.push_back(HydrusID("aigokeajfiamchgfjpmjamec"));

	std::sort(ids.begin(), ids.end());

	char c[24];
	for (HydrusID id : ids)
	{
		id.idCStr(c);
		printf("%s\n", c);
		memset(c, 0, 24);
	}


	/*auto page = new HydrusThumbnailView(0,0,1000,500);

	// Generate test data
	QSet<HydrusID> fakeIdList;
	//for (HydrusID i = 0; i < 10000; i++)
	//	fakeIdList.insert(i);

	// Set random data
	page->setItems(fakeIdList);
	page->setBackgroundBrush(Qt::red);
	page->resize(1000, 500);

	page->show();*/

	//return a.exec();
}
