#include "handler_server.hpp"

SQLite::Database db("main.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE); 
std::mutex mutex;

std::string Handler_server::processing(const std::string& data)
{
	try
	{
		tinyxml2::XMLDocument doc;
		doc.Parse(data.c_str());

		auto type = doc.FirstChildElement("type");
		if (type)
		{
			auto body = doc.FirstChildElement("body");
			if (type->GetText() == std::string("authorization"))
			{
				return this->auth(body->FirstChildElement("login")->GetText(), body->FirstChildElement("password")->GetText());
			}
			else if (type->GetText() == std::string("registration"))
			{
				return this->reg(body->FirstChildElement("login")->GetText(), body->FirstChildElement("password")->GetText(),
					body->FirstChildElement("img_type")->GetText(), body->FirstChildElement("img")->GetText());
			}
			else if (type->GetText() == std::string("patterns"))
			{
				return this->patterns();
			}
			else if (type->GetText() == std::string("result"))
			{
				return this->result(body->FirstChildElement("login")->GetText(), body->FirstChildElement("password")->GetText(),
					body->FirstChildElement("pattern")->GetText(), body->FirstChildElement("result")->GetText());
			}
			else
			{
				std::cout << "ERROR parse xml: " <<  " type " << type->GetText() << " not found" << std::endl;
			}
		}
		else
		{
			std::cout << "ERROR parse xml: not found tag type" << std::endl;
		}
		return this->uncorrect();
	}
	catch (...)
	{
		std::cout << "ERROR parse xml: critical error" << std::endl;
		return this->uncorrect();
	}
}

std::string Handler_server::auth(const std::string& login, const std::string& password)
{
	if (this->is_correct_auth(login, password))
	{
		SQLite::Statement user_id(db, "SELECT id FROM User WHERE login = ? and password = ?");
		user_id.bind(1, login);
		user_id.bind(2, password);
		user_id.executeStep();
		std::string id{ user_id.getColumn(0).getString() };

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.1\" encoding=\"UTF-8\"");
		doc.InsertEndChild(decl);
		tinyxml2::XMLElement* body = doc.NewElement("body");

		tinyxml2::XMLElement* answer = doc.NewElement("answer");
		answer->SetText("correct");
		body->InsertEndChild(answer);

		tinyxml2::XMLElement* img = doc.NewElement("img");
		SQLite::Statement path_query(db, "SELECT path_to_image FROM User WHERE login = '" + login + "'");
		path_query.executeStep();
		std::string path_to_file{ path_query.getColumn(0).getString() };

		img->SetText(this->encode_file(path_to_file).c_str());
		body->InsertEndChild(img);

		tinyxml2::XMLElement* img_type_xml = doc.NewElement("img_type");
		img_type_xml->SetText(this->delim(path_to_file, ".")[1].c_str());
		body->InsertEndChild(img_type_xml);

		SQLite::Statement c_test(db, "SELECT COUNT(*) FROM User_test WHERE id_user = ?");
		c_test.bind(1, id);
		c_test.executeStep();
		tinyxml2::XMLElement* count_test = doc.NewElement("count_test");
		int counter{ c_test.getColumn(0).getInt() };
		count_test->SetText(c_test.getColumn(0).getText());
		body->InsertEndChild(count_test);

		if (counter)
		{
			tinyxml2::XMLElement* tests = doc.NewElement("tests");

			SQLite::Statement res_tests(db, "SELECT * FROM User_test WHERE id_user = ?");
			res_tests.bind(1, id);

			for (int i = 0; i < counter; i++)
			{
				res_tests.executeStep();

				SQLite::Statement name(db, "SELECT name FROM Pattern WHERE id = ?");
				name.bind(1, res_tests.getColumn(2).getText());
				name.executeStep();

				tinyxml2::XMLElement* xml_name = doc.NewElement("name");
				xml_name->SetText(name.getColumn(0).getText());
				tests->InsertEndChild(xml_name);

				tinyxml2::XMLElement* result = doc.NewElement("result");
				result->SetText(res_tests.getColumn(3).getText());
				tests->InsertEndChild(result);
			}

			body->InsertEndChild(tests);
		}

		doc.InsertEndChild(body);

		tinyxml2::XMLPrinter printer;
		doc.Print(&printer);
		return printer.CStr();
	}
	else
	{
		return this->uncorrect();
	}
}

std::string Handler_server::reg(const std::string& login, const std::string& password, const std::string& img_type, const std::string& img)
{
	SQLite::Statement query(db, "SELECT COUNT(id) FROM User WHERE login = ?");
	query.bind(1, login);
	query.executeStep();
	
	if (query.getColumn(0).getInt() == 0) /*нет такого логина*/
	{
		std::string path_to_file{ "images/users_images/" + login + "." + img_type };
		std::ofstream tmp_file(path_to_file);
		tmp_file.close();

		std::ofstream file(path_to_file, std::ios::out | std::ios::binary);
		auto data{ base64_decode(img) };
		for (size_t i = 0; i < data.size(); i++)
		{
			file << data[i];
		}
		file.close();

		mutex.lock();
		db.exec("INSERT INTO User (login, password, path_to_image) VALUES('" + login + "', '" + password + "', '" + path_to_file + "')");
		mutex.unlock();

		return this->correct();
	}
	else
	{
		return this->uncorrect();
	}
}

std::string Handler_server::patterns()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.1\" encoding=\"UTF-8\"");
	doc.InsertEndChild(decl);
	tinyxml2::XMLElement* body = doc.NewElement("body");

	int counter = db.execAndGet("SELECT COUNT(*) FROM Pattern");
	tinyxml2::XMLElement* counter_xml = doc.NewElement("count_pattern");
	counter_xml->SetText(counter);
	body->InsertEndChild(counter_xml);

	tinyxml2::XMLElement* pattern_list_xml = doc.NewElement("pattern_list");

	SQLite::Statement data(db, "SELECT * FROM Pattern");

	while (data.executeStep())
	{
		tinyxml2::XMLElement* pattern_xml = doc.NewElement("pattern");

		int id = data.getColumn(0).getInt();
		const char* name = data.getColumn(1).getText();
		const char* desc = data.getColumn(2).getText();
		const char* code = data.getColumn(3).getText();
		const char* path_to_image = data.getColumn(4).getText();

		tinyxml2::XMLElement* name_xml = doc.NewElement("name");
		name_xml->SetText(name);
		pattern_xml->InsertEndChild(name_xml);

		tinyxml2::XMLElement* desc_xml = doc.NewElement("description");
		desc_xml->SetText(desc);
		pattern_xml->InsertEndChild(desc_xml);

		tinyxml2::XMLElement* code_xml = doc.NewElement("code");
		code_xml->SetText(code);
		pattern_xml->InsertEndChild(code_xml);

		tinyxml2::XMLElement* img_xml = doc.NewElement("img");
		img_xml->SetText(this->encode_file(path_to_image).c_str());
		pattern_xml->InsertEndChild(img_xml);

		tinyxml2::XMLElement* img_type_xml = doc.NewElement("img_type");
		img_type_xml->SetText(this->delim(path_to_image, ".")[1].c_str());
		pattern_xml->InsertEndChild(img_type_xml);
		
		tinyxml2::XMLElement* test_list_xml = doc.NewElement("test_list");

		SQLite::Statement test_data(db, "SELECT * FROM Pattern_test WHERE id_pattern = " + std::to_string(id));
		while (test_data.executeStep())
		{
			tinyxml2::XMLElement* test_xml = doc.NewElement("test");

			const char* question = test_data.getColumn(1).getText();
			int type = test_data.getColumn(2).getInt();

			tinyxml2::XMLElement* question_xml = doc.NewElement("question");
			question_xml->SetText(question);
			test_xml->InsertEndChild(question_xml);

			tinyxml2::XMLElement* type_xml = doc.NewElement("id_description");
			type_xml->SetText(type);
			test_xml->InsertEndChild(type_xml);

			const char* correct_answer = test_data.getColumn(7).getText();

			if (type == 1)
			{
				const char* a1 = test_data.getColumn(3).getText();
				const char* a2 = test_data.getColumn(4).getText();
				const char* a3 = test_data.getColumn(5).getText();
				const char* a4 = test_data.getColumn(6).getText();

				tinyxml2::XMLElement* a1_xml = doc.NewElement("a1");
				a1_xml->SetText(a1);
				test_xml->InsertEndChild(a1_xml);

				tinyxml2::XMLElement* a2_xml = doc.NewElement("a2");
				a2_xml->SetText(a2);
				test_xml->InsertEndChild(a2_xml);

				tinyxml2::XMLElement* a3_xml = doc.NewElement("a3");
				a3_xml->SetText(a3);
				test_xml->InsertEndChild(a3_xml);

				tinyxml2::XMLElement* a4_xml = doc.NewElement("a4");
				a4_xml->SetText(a4);
				test_xml->InsertEndChild(a4_xml);
			}

			tinyxml2::XMLElement* correct_xml = doc.NewElement("correct_answer");
			correct_xml->SetText(correct_answer);
			test_xml->InsertEndChild(correct_xml);
			test_list_xml->InsertEndChild(test_xml);
		}

		pattern_xml->InsertEndChild(test_list_xml);
		pattern_list_xml->InsertEndChild(pattern_xml);
	}

	body->InsertEndChild(pattern_list_xml);
	doc.InsertEndChild(body);

	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);
	
	return printer.CStr();
}

std::string Handler_server::result(const std::string& login, const std::string& password, const std::string& pattern, const std::string& result)
{
	if (this->is_correct_auth(login, password))
	{
		int id{ db.execAndGet("SELECT id FROM User WHERE login = '" + login + "'") };
		int id_test{ db.execAndGet("SELECT id FROM Pattern WHERE name = '" + pattern + "'") };

		if (!id && !id_test)
			return this->uncorrect();

		mutex.lock();
		if ((int)db.execAndGet("SELECT COUNT(*) FROM User_test WHERE id_user = " + std::to_string(id) + " AND id_pattern = " + std::to_string(id_test)) == 0)
		{
			db.exec("INSERT INTO User_test (id_user, id_pattern, count_correct) VALUES(" + std::to_string(id) + ", " + std::to_string(id_test) + ", " + result + ")");
		}
		else
		{
			db.exec("UPDATE User_test SET count_correct = " + result + " WHERE id_user = " + std::to_string(id) + " AND id_pattern = " + std::to_string(id_test));
		}
		mutex.unlock();

		return this->correct();
	}
	else
	{
		return this->uncorrect();
	}
}

bool Handler_server::is_correct_auth(const std::string& login, const std::string& password)
{
	SQLite::Statement query(db, "SELECT COUNT(*) FROM User WHERE login = ? and password = ?");
	query.bind(1, login);
	query.bind(2, password);

	query.executeStep();
	return query.getColumn(0).getInt();
}

std::string Handler_server::uncorrect()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.1\" encoding=\"UTF-8\"");
	doc.InsertEndChild(decl);
	tinyxml2::XMLElement* body = doc.NewElement("body");

	tinyxml2::XMLElement* answer = doc.NewElement("answer");
	answer->SetText("uncorrect");
	body->InsertEndChild(answer);

	doc.InsertEndChild(body);

	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}

std::string Handler_server::correct()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.1\" encoding=\"UTF-8\"");
	doc.InsertEndChild(decl);
	tinyxml2::XMLElement* body = doc.NewElement("body");

	tinyxml2::XMLElement* answer = doc.NewElement("answer");
	answer->SetText("correct");
	body->InsertEndChild(answer);

	doc.InsertEndChild(body);

	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}

std::vector<std::string> Handler_server::delim(std::string str, std::string delim)
{
	std::vector<std::string> arr;
	size_t prev = 0;
	size_t next;
	size_t delta = delim.length();

	while ((next = str.find(delim, prev)) != std::string::npos)
	{
		arr.push_back(str.substr(prev, next - prev));
		prev = next + delta;
	}

	arr.push_back(str.substr(prev));

	return arr;
}

std::string Handler_server::encode_file(const std::string& path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		throw "not open file: " + path;
	file.seekg(0, file.end);
	size_t length = file.tellg();
	file.seekg(0, file.beg);

	std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return base64_encode(data);
}
