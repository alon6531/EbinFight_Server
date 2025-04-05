#pragma once
#include "Globals.h"

class User
{
public:
	User(sf::TcpSocket& client);


	void SetName(const std::string& name);

	const std::string& GetName() const;



	void PrintUser() const;


public:
	sf::TcpSocket* m_client;

private:
	
	std::string m_name;
	//class Entity entity;

};

