#include "AnimeList.h"
AnimeList::AnimeList()
{
	CtrlLayout(*this, "Anime List");
	CtrlLayout(infoTab);
	CtrlLayout(addTab);
	CtrlLayout(clndr);
	CtrlLayout(parameters);
	CtrlLayout(dlg, "Modify Items");
	CtrlLayout(test);
	
	test.testList.AddColumn("1");
	test.testList.AddColumn("2");
	
	Icon(AImage::icon());

	animeList.Add(infoTab, "Information");
	animeList.Add(addTab, "Add Items");
	animeList.Add(test, "Test Information");

	parTab.Add(clndr, "Calendar");
	parTab.Add(parameters, "Parameters");

	addTab.complete.Add("Yes");
	addTab.complete.Add("No");
	addTab.complete.Add("New");
	addTab.complete.SetIndex(1);
	addTab.episodes.SetText("0");

	drop.Add("Yes");
	drop.Add("No");
	drop.Add("New");

	dlg.complete.Add("Yes");
	dlg.complete.Add("No");
	dlg.complete.Add("New");

	parameters.seasons.Add("All Seasons");
	parameters.seasons.Add("Winter");
	parameters.seasons.Add("Spring");
	parameters.seasons.Add("Summer");
	parameters.seasons.Add("Autumn");
	parameters.seasons.SetIndex(0);
	
	parameters.years.Add("All Years");
	parameters.years.SetIndex(0);
	
	parameters.complete.Add("All");
	parameters.complete.Add("Yes");
	parameters.complete.Add("No");
	parameters.complete.Add("New");
	parameters.complete.SetIndex(0);

	parameters.looking.Add("All");
	parameters.looking.Add("Looking");
	parameters.looking.Add("Don't Looking");
	parameters.looking.SetIndex(0);

	CreateArray();

	seriesDate.AddColumn("Ep");
	seriesDate.AddColumn("Release(m/d/y)");
	seriesDate.ColumnWidths("2 5");
	seriesDate.EvenRowColor(WhiteGray());

	listName.WhenLeftClick = THISBACK(Change);
	listName.WhenAcceptEdit = THISBACK(EditValue);
	listName.WhenBar = THISBACK(RightMenu);
	seriesDate.WhenLeftClick = THISBACK(ChangeSeries);
	dateSeries.WhenAction = THISBACK(ModifyDate);
	clndr.calendar.WhenAction = THISBACK(Test);
	parameters.years.WhenAction = THISBACK(Sorts);
	parameters.seasons.WhenAction = THISBACK(Sorts);
	parameters.complete.WhenAction = THISBACK(Sorts);
	parameters.looking.WhenAction = THISBACK(Sorts);
	
	addTab.add <<= THISBACK(Add);
	infoTab.modify <<= THISBACK(OpenModify);
	dlg.modify <<= THISBACK(Modify);
	dlg.cancel <<= THISBACK(DoClose);
	infoTab.viewPlus <<= THISBACK(PlusViews);
	infoTab.viewMinus <<= THISBACK(MinusViews);
	moved <<= THISBACK(ModifySeries);
	parameters.reset <<= THISBACK(ResetParameters);
	
	fs.AllFilesType();
	
	menu.Set(THISBACK(MainMenu));

	infoTab.viewPlus.SetImage(AImage::plus());
	infoTab.viewMinus.SetImage(AImage::minus());
}

void AnimeList::Test()
{
	if(listName.IsCursor())
		RowColor(listName.GetCursor());
}
//меню файл
void AnimeList::FileMenu(Bar& bar)
{
	bar.Add("Open", CtrlImg::open(), THISBACK(Open));
	bar.Add("Save", CtrlImg::save(), THISBACK(Save));
	bar.Add("Save As...", CtrlImg::save_as(), THISBACK(SaveAs));
	bar.Separator();
	bar.Add("Close File", THISBACK(CloseFile));
	bar.Separator();
	bar.Add("Last File", THISBACK(OpenLastFile));
	bar.Separator();
	bar.Add("Print to File", CtrlImg::print(), THISBACK(PrintFile));
	bar.Separator();
	bar.Add("Exit", THISBACK(Quit));
}
//главное меню
void AnimeList::MainMenu(Bar& bar)
{
	bar.Add("File", THISBACK(FileMenu));
	bar.Add("Full Screen", THISBACK(FullSizeWindow));
	bar.Add("Test", THISBACK(Test));
}
//правая клавиша
void AnimeList::RightMenu(Bar& bar)
{
	bar.Add("Edit Row", THISBACK(OpenModify));
	bar.Add("Remove Row", THISBACK(Remove));
}
//добавление нового элемента
void AnimeList::Add()
{
	if(addTab.complete == "Yes")
	   cell.complete = "Yes";
	else
		if(addTab.complete == "No")
			cell.complete = "No";
		else cell.complete = "New";
	cell.name = ~addTab.name;
	cell.nameRus = ~addTab.nameRus;
	cell.episodes = ~addTab.episodes;
	cell.views = 0;
	cell.Release = ~addTab.dateRelease;
	cell.releaseDate = "";
	mainArray.Add(cell);
	listName.Add(cell.complete, ~addTab.name, ~addTab.nameRus, ~addTab.episodes, 0, ~addTab.dateRelease, "",mainArray.GetCount() - 1);
	
	addTab.complete.SetIndex(1);
	addTab.name.Clear();
	addTab.nameRus.Clear();
	addTab.episodes.SetText("0");
	addTab.dateRelease.Clear();
	
	status.Set(Format("Items: %d. Items Add to List", listName.GetCount()));
	RowColor(listName.GetCount()-1);
}
//составление списка выходов серий
void AnimeList::AnimeSeries()
{
	int episodes;
	Date release, today;
	today = GetSysDate();
	if(listName.IsCursor()){
		seriesDate.Clear();
		episodes = listName.Get(Episodes);
		release = listName.Get(Release);
		int i = 0;
		int j = 1;
		int series = 0;
		
		if(seriesArray.GetCount() > 0)
			while(i == 0) {
				if( j == seriesArray[series].number)
				{
					seriesDate.Add(seriesArray[series].number, seriesArray[series].release);
					release = seriesArray[series].release;
					operator+=(release, 7);
					if(series < seriesArray.GetCount()-1)
						series++;
				}
				else
				{
					seriesDate.Add(j, release);
					operator+=(release, 7);
				}
				j++;
				if(j > episodes)
					i++;
			}
		else
			while(i == 0) { 
				seriesDate.Add(j, release);
				operator+=(release, 7);
				j++;
				if( j  > episodes)
					i = 1;
			}
			
		for(int i=0;i<seriesArray.GetCount();i++)
			test.testList.Add(seriesArray[i].number, seriesArray[i].release);
	}
}
//выбор в главном массиве
void AnimeList::Change()
{
	if(listName.IsCursor()) {
		infoTab.complete.SetLabel(AsString(listName.Get(Complete)));
		infoTab.name.SetLabel(AsString(listName.Get(Named)));
		infoTab.nameRus.SetLabel(AsString(listName.Get(NamedRus)));
		infoTab.episodes.SetLabel(AsString(listName.Get(Episodes)));
		infoTab.views.SetLabel(AsString(listName.Get(Views)));
		infoTab.dateRelease.SetLabel(AsString(listName.Get(Release)));
		test.testStr.SetText(AsString((listName.Get(SeriesRelease))));
		
		if(parameters.checkSave)
		{
			String text;
			int v = listName.Get(Views);
			v++;
			text = Format(AsString(listName.Get(Named))+ " %d", v);
			copy.SetText(text);
			copy.SelectAll();
			copy.Copy();
		};
		
		seriesArray.Clear();
		test.testList.Clear();
		//заполнение массива релизов серий из поля SeriesRelease главного массива (парсер строки)
		String xml = listName.Get(SeriesRelease);
		String number, date;
		int pos;
		while(xml.GetCount() > 0){ 
			pos = xml.Find("-");
			if(pos > 0)
			{
			number = String(xml, pos);
			xml.Remove(0, pos + 1);
			date = String(xml, 10);
			xml.Remove(0, 11);
			series.number = StrInt(number);
			StrToDate(series.release, date);
			seriesArray.Add(series);
			}
		}
			
		if(listName.Get(Complete) != "Yes")
			AnimeSeries();
		else
			seriesDate.Clear();
	}
}
//выбор в массиве релизов серий
 void AnimeList::ChangeSeries()
{
	if(seriesDate.IsCursor())
	{
		dateSeries.SetData(seriesDate.Get(1));
	}
}
//закрывает открытый файл и очищает все массивы
void AnimeList::CloseFile()
{
	listName.Clear();
	dateSeries.Clear();
	Title("Anime List");
	infoTab.name.SetLabel("");
	infoTab.nameRus.SetLabel("");
	infoTab.complete.SetLabel("");
	infoTab.episodes.SetLabel("");
	infoTab.views.SetLabel("");
	infoTab.dateRelease.SetLabel("");
	filename.Clear();
	mainArray.Clear();
	seriesDate.Clear();
}
//Создание заголовков главного массива
void AnimeList::CreateArray()
{
	for (int i = 0; i < Items_name_len; i++)
		listName.AddColumn(AsString(Items_name[i]));
	listName.AddCtrl(seriesDate);
	listName.ColumnWidths("3 10 10 3 2 4");
	listName.ColumnAt(Complete).Edit(drop);
	listName.ColumnAt(Named).Edit(name);
	listName.ColumnAt(NamedRus).Edit(nameRus);
	listName.ColumnAt(Episodes).Edit(ep1);
	listName.ColumnAt(Views).Edit(ep2);
	listName.ColumnAt(Release).Edit(date);
	listName.AllSorting();
	listName.HeaderObject().ShowTab(SeriesRelease, false);
	listName.HeaderObject().ShowTab(ID, false);
}
//закрытие диалога
void AnimeList::DoClose()
{
	dlg.Close();
}
//Изменение значений массива при редактировании в главном массиве
void AnimeList::EditValue()
{
	if(listName.IsCursor())
	{
		cell.complete = listName.Get(Complete);
		cell.name = listName.Get(Named);
		cell.nameRus = listName.Get(NamedRus);
		cell.episodes = listName.Get(Episodes);
		cell.views = listName.Get(Views);
		cell.Release = listName.Get(Release);
		cell.releaseDate = listName.Get(SeriesRelease);
		
		mainArray.Set(listName.Get(ID), cell);
	}
}
//Перевод приложения в/из полноэкранный режим
void AnimeList::FullSizeWindow() 
{
	if(size)
	{
		size = false;
		FullScreen(size);
	} else
	{
		size = true;
		FullScreen(size);
	}
}
//уменьшить количество просмотренных серий
void AnimeList::MinusViews()
{
	if(listName.IsCursor())
	{
		int a;
		a = listName.Get(Views);
		if (a > 0)
		{
			a--;
			listName.Set(Views, a);
			mainArray[listName.Get(ID)].views = a;
			infoTab.views.SetLabel(AsString(a));
		}
		RowColor(listName.GetCursor());
	}
}
//применение изменений при вызове диалога модификации
void AnimeList::Modify()
{	
	if(listName.IsCursor())
	{
		listName.Set(Complete, ~dlg.complete);
		listName.Set(Named, ~dlg.name);
		listName.Set(NamedRus, ~dlg.nameRus);
		listName.Set(Episodes, ~dlg.episodes);
		listName.Set(Views, ~dlg.views);
		listName.Set(Release, ~dlg.dateRelease);
		listName.Set(SeriesRelease, ~dlg.seriesRelease);
		
		cell.complete = ~dlg.complete;
		cell.name = ~dlg.name;
		cell.nameRus = ~dlg.nameRus;
		cell.episodes = ~dlg.episodes;
		cell.views = ~dlg.views;
		cell.Release = ~dlg.dateRelease;
		cell.releaseDate = ~dlg.seriesRelease;
		
		mainArray.Set(listName.Get(ID), cell);
	}
	dlg.Close();
	Change();
}
//изменение списка релизов серии при переносе релиза одной из них
void AnimeList::ModifyAnimeSeries()
{	
	if(seriesDate.IsCursor())
	{
		int i = seriesDate.GetCount() - seriesDate.GetCursor() - 1;
		for(int j = 0; j < i; j++)
			seriesDate.Remove(seriesDate.GetCursor() + 1);
		if(listName.IsCursor())
		{
			int episodes = listName.Get(Episodes);
			Date release = seriesDate.Get(1);
			int i = 0;
			int j = seriesDate.Get(0);
			j++;
			operator+=(release, 7);
			while(i == 0) {
				seriesDate.Add(j, release);
				operator+=(release, 7);
				j++;
				if( j  > episodes)
					i = 1;
			}
		}
	}
}
//ручное задание переноса серии
void AnimeList::ModifyDate()
{
	if(seriesDate.IsCursor())
	{
		seriesDate.Set(1, ~dateSeries);
		series.number = seriesDate.Get(Series);
		series.release = ~dateSeries;
		SeriesEdit();
		ModifyAnimeSeries();
	}
	Change();
	ChangeSeries();
}
//перенос релиза серии на 7 дней
void AnimeList::ModifySeries()
{
	Date release;
	if(seriesDate.IsCursor())
	{
		release = ~dateSeries;
		operator+=(release, 7);
		seriesDate.Set(1, release);
		series.number = seriesDate.Get(Series);
		series.release = release;
		SeriesEdit();
		ModifyAnimeSeries();
	}
	Change();
	ChangeSeries();
}
//открытие файла из системы
void AnimeList::Open()
{
	if(!fs.ExecuteOpen()) return;
	filename = fs;
	OpenFile();
}
//открытие файла после выбора его в системе
void AnimeList::OpenFile()
{
	mainArray.Clear();
	listName.Clear();
	try {
			String d = LoadFile(filename);
			XmlParser p(d);
			while(!p.IsTag())
				p.Skip();
			p.PassTag("ListView");
			while(!p.IsEof()) {
				if(p.Tag("item")) {
					while(!p.End()) {
						if(p.Tag("complete"))
						{
							String complete = p.ReadText();
							if(complete == "Yes")
							   cell.complete = "Yes";
							else
								if(complete == "No")
									cell.complete = "No";
								else cell.complete = "New";
						}
						else
						if(p.Tag("name"))
							cell.name = p.ReadText();
						else
						if(p.Tag("russian"))
							cell.nameRus = p.ReadText();
						else
						if(p.Tag("episodes"))
							cell.episodes = StrInt(p.ReadText());
						else
						if(p.Tag("views"))
							cell.views = StrInt(p.ReadText());
						else
						if(p.Tag("Release"))
							StrToDate(cell.Release, p.ReadText());
						else
						if(p.Tag("series"))
							cell.releaseDate = p.ReadText();
						else {
							p.Skip();
							continue;
						}
						p.PassEnd();
					}
					mainArray.Add(cell);
					listName.Add(cell.complete, cell.name, cell.nameRus, cell.episodes, cell.views, cell.Release, cell.releaseDate, mainArray.GetCount() - 1);
					RowColor(listName.GetCount()-1);
				}
				else 
					p.Skip();
			}
		}
		catch(XmlError) {
			Exclamation("Error Readong the Input File");
		} 
		
	SetupYears();
	
	Title("Anime List | " + filename);
		
	String iniFile;
	iniFile = GetDataFile("AnimeList.xml");
	String xmlIni;
	xmlIni <<
			XmlTag("File").Text(AsString(filename));
	SaveFile(iniFile, XmlDoc("FileList", xmlIni));
	
	status.Set(Format("Items: %d. List Open", listName.GetCount()));
}
//открытие последнего открытого файла
void AnimeList::OpenLastFile()
{
	String iniFile = GetDataFile("AnimeList.xml");
	String d = LoadFile(iniFile);
	XmlParser p(d);
	while(!p.IsTag())
		p.Skip();
	p.PassTag("FileList");
		if(p.Tag("File"))
			filename = p.ReadText();
	OpenFile();
}
//вызов диалога модификации элемента
void AnimeList::OpenModify()
{	
	if(dlg.IsOpen())
			dlg.Close();
		else
		{
			dlg.Open(this);
			if(listName.IsCursor()) 
			{
				if(listName.Get(Complete) == "Yes")
					dlg.complete.SetIndex(0);
				else
					if(listName.Get(Complete) == "No")
						dlg.complete.SetIndex(1);
					else dlg.complete.SetIndex(2);
			
				dlg.name.SetText(AsString(listName.Get(Named)));
				dlg.nameRus.SetText(AsString(listName.Get(NamedRus)));
				dlg.episodes.SetText(AsString(listName.Get(Episodes)));
				dlg.views.SetText(AsString(listName.Get(Views)));
				dlg.dateRelease.SetText(AsString(listName.Get(Release)));
				dlg.views.MinMax(0, listName.Get(Episodes));
				dlg.seriesRelease.SetText(AsString(listName.Get(SeriesRelease)));
			}
		}
}
//увеличение количества просмотренных серий
void AnimeList::PlusViews()
{
	if(listName.IsCursor())
	{
		int a;
		int b;
		a = listName.Get(Views);
		b = listName.Get(Episodes);
		if (a < b)
		{
			a++;
			listName.Set(Views, a);
			mainArray[listName.Get(ID)].views = a;
			infoTab.views.SetLabel(AsString(a));
		}
		RowColor(listName.GetCursor());
	}
}
//Вывод в файл список в формате Оригинал - Русское - количество эпизодов
void AnimeList::PrintFile()
{
	if(!fs.ExecuteSaveAs()) return;
	String printfile = fs;
	
	String xml, name, namerus, episodes;
	for(int i = 0; i < listName.GetCount(); i++)
	{	
		xml += Format(AsString(listName.Get(i, Named)) + " - " + AsString(listName.Get(i, NamedRus)) + " - Episodes: %d", listName.Get(i, Episodes));
		RawCat(xml, "\r\n");
	}
	
	if(!SaveFile(printfile, xml))
		Exclamation("Error Saving the File");
}
//Удаление строки
void AnimeList::Remove()
{
	if(listName.IsCursor())
	{
		mainArray.Remove(listName.Get(ID));
		seriesDate.Clear();
		infoTab.name.SetLabel("");
		infoTab.nameRus.SetLabel("");
		infoTab.complete.SetLabel("");
		infoTab.episodes.SetLabel("");
		infoTab.views.SetLabel("");
		infoTab.dateRelease.SetLabel("");
		Sorts();
	}
}
//сброс параметров сортировки
void AnimeList::ResetParameters()
{
	parameters.years.SetIndex(0);
	parameters.seasons.SetIndex(0);
	parameters.complete.SetIndex(0);
	parameters.looking.SetIndex(0);
	Sorts();
}
//задание цвета строки
void AnimeList::RowColor(int row)
{
	Date release;
	Array<CellSeries> array;
	Date today = GetSysDate();
	String xml = listName.Get(row, SeriesRelease);
	String number, date;
	int ep = listName.Get(row, Episodes);
	int ser;
	if(xml.GetCount() > 0)
	{
		int pos;
		while(xml.GetCount() > 0){ 
			pos = xml.Find("-");
			if(pos > 0)
			{
				number = String(xml, pos);
				xml.Remove(0, pos + 1);
				date = String(xml, 10);
				xml.Remove(0, 11);
				series.number = StrInt(number);
				StrToDate(series.release, date);
				array.Add(series);
			}
		}
		release = array[array.GetCount()-1].release;
		if((ep == array[array.GetCount()-1].number) & (today < release))
			ser = array[array.GetCount()-1].number - 1;
		else
			ser = ((today - release) / 7) + array[array.GetCount()-1].number;
	} else
	{
		release = listName.Get(row, Release);
		ser = ((today - release) / 7) + 1;
	}
	int views = listName.Get(row, Views);
	if(views == ep) //полностью просмотрено
		for(int i=0;i<listName.GetColumnCount();i++)
			listName.SetDisplay(row, i, Single<ColumnBlue>());
	else
		if((views < ep) & (views != 0) & (views == ser)) //нет новых серий, в просмотре
			for(int i=0;i<listName.GetColumnCount();i++)
				listName.SetDisplay(row, i, Single<ColumnGreen>());
		else
			if((views < ep) & (views != 0) & (views < ser)) //есть новые серии, в просмотре
				for(int i=0;i<listName.GetColumnCount();i++)
					listName.SetDisplay(row, i, Single<ColumnRed>());
			else
				if(views == 0) //нет просмотренных серий
					for(int i=0;i<listName.GetColumnCount();i++)
						listName.SetDisplay(row, i, Single<ColumnDefault>());
}
//сохранение списка в файл
void AnimeList::Save()
{
	if(IsEmpty(filename)) {
		SaveAs();
		return;
	}
	String xml;
	for(int i = 0; i < mainArray.GetCount(); i++)
		xml <<
			XmlTag("item") (
				XmlTag("complete").Text(mainArray[i].complete) +
				XmlTag("name").Text(mainArray[i].name) +
				XmlTag("russian").Text(mainArray[i].nameRus) + 
				XmlTag("episodes").Text(AsString(mainArray[i].episodes)) +
				XmlTag("views").Text(AsString(mainArray[i].views)) +
				XmlTag("Release").Text(AsString(mainArray[i].Release)) +
				XmlTag("series").Text(mainArray[i].releaseDate)
			);
	if(!SaveFile(filename, XmlDoc("ListView", xml)))
		Exclamation("Error Saving the File");
	
	status.Set(Format("Items: %d. List Save", mainArray.GetCount()));
}
//сохранение списка в новый файл
void AnimeList::SaveAs()
{
	if(!fs.ExecuteSaveAs()) return;
	filename = fs;
	Save();
}
//запись номера серии и даты релиза при переносе 
void AnimeList::SeriesEdit()
{
	int a=0;
	if(seriesArray.GetCount() > 0)
		for(int i=0;i<seriesArray.GetCount(); i++)
		{
			if(series.number == seriesArray[i].number)
			{
				seriesArray[i].release = series.release;
				a = 1;
				break;
			}
		}
	if(a == 0)
	{
		seriesArray.Add(series);
		if(seriesArray.GetCount() > 1)
		{
			int last = seriesArray.GetCount() - 1;
			for(int i=0;i<seriesArray.GetCount();i++)
				if(seriesArray[last].number < seriesArray[i].number)
				{
					seriesArray.Move(last, i);
					break;
				}
		}
	}
	
	if(listName.IsCursor())
	{
		String xml;
		for(int i=0; i<seriesArray.GetCount(); i++)
			xml += Format("%d-"+AsString(seriesArray[i].release)+";", seriesArray[i].number);
		listName.Set(SeriesRelease, xml);
		mainArray[listName.Get(ID)].releaseDate = xml;
	}
}

//установка годов в которые среди списка есть релизы
void AnimeList::SetupYears()
{
	parameters.years.Clear();
	parameters.years.Add("All Years");
	parameters.years.SetIndex(0);
	Array<int> year;
	int last;
	bool a = false;
	if(!mainArray.IsEmpty())
	{
		for(int i=0;i<mainArray.GetCount();i++)
		{
			a = false;
			if(year.GetCount() == 0)
			{
				year.Add(mainArray[i].Release.year);
				last = year.GetCount() - 1;
			}
			else
			{
				for(int j=0;j<year.GetCount();j++)
					if(mainArray[i].Release.year == year[j])
					{	
						a = true;
						break;
					}
				if(!a)
				{
					year.Add(mainArray[i].Release.year);
					last = year.GetCount() - 1;
				}
				for(int i=0;i<year.GetCount();i++)
					if(year[last]<year[i])
					{
						year.Swap(i, last);
					}
			}
		}

		for(int i=year.GetCount()-1;i>=0;i--)
			parameters.years.Add(year[i]);
	}
}
//сортировка
void AnimeList::Sorts()
{
	SortsYears();
	SortsComplete();
	SortsSeasons();
	SortsLooks();
	status.Set(Format("Items: %d. Viewing Items: %d.", mainArray.GetCount(), listName.GetCount()));
	for(int i=0;i<listName.GetCount();i++)
	    RowColor(i);
}
//сортировка по статусу выхода
void AnimeList::SortsComplete()
{
	if(parameters.complete.GetValue() == "Yes")
	{
		int i=0;
		while(i<listName.GetCount())
		{
			if(listName.Get(i, Complete) != "Yes")
				listName.Remove(i);
			else i++;
		}
	} else
		if(parameters.complete.GetValue() == "No")
		{
			int i=0;
			while(i<listName.GetCount())
			{
				if(listName.Get(i, Complete) != "No")
					listName.Remove(i);
				else i++;
			}
		} else
			if(parameters.complete.GetValue() == "New")
			{
				int i=0;
				while(i<listName.GetCount())
				{
					if(listName.Get(i, Complete) != "New")
					listName.Remove(i);
					else i++;
				}
			}
}
//сортировка просмотра
void AnimeList::SortsLooks()
{
	if(parameters.looking.GetIndex() == 1) //Looking
	{
		int i=0;
		while(i<listName.GetCount())
		{
			int views = mainArray[listName.Get(i, ID)].views;
			if(views == 0)
				listName.Remove(i);
			else i++;
		}
	} else
		if(parameters.looking.GetIndex() == 2) //Don't Looking
		{
			int i=0;
			while(i<listName.GetCount())
			{
				int views = mainArray[listName.Get(i, ID)].views;
				if(views != 0)
					listName.Remove(i);
				else i++;
			}
		}
}
//сортировка по сезонам
void AnimeList::SortsSeasons()
{
	if(parameters.seasons.GetIndex() == 1) //Winter
	{
		int i=0;
		while(i < listName.GetCount())
		{
			int month = mainArray[listName.Get(i, ID)].Release.month;
			if(month > 2 && month < 12)
				listName.Remove(i);
			else i++;
		}
	}
		if(parameters.seasons.GetIndex() == 2) //Spring
		{
			int i=0;
			while(i < listName.GetCount())
			{
				int month = mainArray[listName.Get(i, ID)].Release.month;
				if(month < 3 || month > 5)
					listName.Remove(i);
				else i++;
			}
		} 
			if(parameters.seasons.GetIndex() == 3) //Summer
			{
				int i=0;
				while(i < listName.GetCount())
				{
					int month = mainArray[listName.Get(i, ID)].Release.month;
					if(month < 6 || month > 8)
						listName.Remove(i);
					else i++;
				}	
			} 
				if(parameters.seasons.GetIndex() == 4) //Autumn
				{
					int i=0;
					while(i < listName.GetCount())
					{
						int month = mainArray[listName.Get(i, ID)].Release.month;
						if(month < 9 || month > 11)
							listName.Remove(i);
						else i++;
					}
				}
}
//построение списка по годам выпуска
void AnimeList::SortsYears()
{
	listName.Clear();
	if(parameters.years.GetIndex() == 0) //All Years
		for(int i=0;i<mainArray.GetCount();i++)
			listName.Add(mainArray[i].complete, mainArray[i].name, mainArray[i].nameRus, mainArray[i].episodes, mainArray[i].views, mainArray[i].Release, mainArray[i].releaseDate, i);
	else
	{
		int year = parameters.years.GetValue();
		for(int i=0;i<mainArray.GetCount();i++)
			if(year == mainArray[i].Release.year)
				listName.Add(mainArray[i].complete, mainArray[i].name, mainArray[i].nameRus, mainArray[i].episodes, mainArray[i].views, mainArray[i].Release, mainArray[i].releaseDate, i);
				
	}
	SortsSeasons();
}
//выход из приложения
void AnimeList::Quit()
{
	Break();
}

GUI_APP_MAIN
{
	AnimeList app;
	app.MinimizeBox();
	app.MaximizeBox();
	app.Sizeable();
	app.Run();
}
