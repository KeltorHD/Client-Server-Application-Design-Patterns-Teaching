#include "handler_server.h"

SQLite::Database db("main.db");

std::string Handler_server::processing(const std::string& data)
{
	tinyxml2::XMLDocument doc;
	doc.Parse(data.c_str());

	auto type = doc.FirstChildElement("type");
	if (type)
	{
		if (type->GetText() == std::string("authorization"))
		{
			auto body = doc.FirstChildElement("body");
			return this->auth(body->FirstChildElement("login")->GetText(), body->FirstChildElement("passv")->GetText());
		}
		else if (type->GetText() == std::string("registration"))
		{

		}
		else if (type->GetText() == std::string("patterns"))
		{

		}
		else if (type->GetText() == std::string("result"))
		{

		}
		else
		{
			std::cout << "ERROR parse xml" << std::endl;
		}
	}
	else
	{
		std::cout << "not type" << std::endl;
	}
	std::cout << "done" << std::endl;
	return "";
}

void Handler_server::init_coonection_bd()
{
}

std::string Handler_server::auth(std::string login, std::string password)
{
	if (this->is_correct_auth(login, password))
	{
		SQLite::Statement user_id(db, "SELECT id FROM User WHERE login = ? and password = ?");
		user_id.bind(1, "admin");
		user_id.bind(2, "admin");
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
		img->SetText("base64");
		body->InsertEndChild(img);

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
				std::cout << res_tests.getColumn(2).getText() << std::endl;
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

	return "";
}

bool Handler_server::is_correct_auth(std::string login, std::string password)
{
	SQLite::Statement query(db, "SELECT COUNT(*) FROM User WHERE login = ? and password = ?");
	query.bind(1, "admin");
	query.bind(2, "admin");

	query.executeStep();
	return query.getColumn(0).getInt();
}
