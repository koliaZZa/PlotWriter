#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Model.Document.h"

using std::vector, std::string;

namespace ARP::Model
{
	// ����� ��������� ����������
	class ApplicationContext : public Object
	{
	public:
		ApplicationContext(string iPath = "ARP.Settings.json");
		~ApplicationContext() = default;

		// ��������� ��������� � ����
		bool Save(string iPath = "ARP.Settings.json");

		// ��������� � json-��������
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// ��������� �� json-���������
		virtual void Load(const rj::Value& doc) override;

		// �������� ���� � ������ ���������
		bool AddRecentFile(string iPath);
		// �������� ������ ��������� ������
		vector<string> GetRecentFiles() const { return recentProjects; };
		// �������� ���� �� ������ ��������� �� �������
		string GetRecentFile(int ind) const { return recentProjects[ind]; };

		// �������� ������ � ������ �������� ����������
		void AddDoc(DocumentPtr iDoc) { openDocs.push_back(iDoc); };
		// ������� ������ �� ������ ��������
		void DelDoc(DocumentPtr iDoc);

	protected:
		vector<string> recentProjects;		// ������ ��������� ������
		vector<DocumentPtr> openDocs;		// ������ �������� ����������
	};
	using ApplicationContextPtr = std::shared_ptr<ApplicationContext>;
}