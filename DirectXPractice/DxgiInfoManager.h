#pragma once

#ifndef DXGIINFOMANAGER_H
# define DXGIINFOMANAGER_H
# include "LvRain.h"
# include <vector>


class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	std::vector<std::string> GetMessages() const;
	void Set() noexcept;

private:
	unsigned long long next = 0u;
	struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};

#endif