#include "User.hpp"


bool User::checkApprovalStatus(){
	return this->approval_;
}

void User::approve(){
	approval_ = true;
}