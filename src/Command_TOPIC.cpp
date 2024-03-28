#include "Command.hpp"

void Command::TOPIC(User &user, std::vector<std::string> &arg) {
	std::cout << "start TOPIC" << std::endl;
	if (arg.empty()) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("TOPIC") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("TOPIC"));
		return;
	} else if (!this->server_.hasChannelName(arg.at(0))) {
		std::cerr << reply_.ERR_NOSUCHCHANNEL(arg.at(0)) << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NOSUCHCHANNEL(arg.at(0)));
		return;
	}
	const Channel &topic_ch = this->server_.getChannel(arg.at(0));
	if (topic_ch.hasMode(Channel::t) &&
		!topic_ch.isChannelOperator(user.getFd())) {
		std::cerr << reply_.ERR_CHANOPRIVSNEEDED(arg.at(0)) << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_CHANOPRIVSNEEDED(arg.at(0)));
		return;
	} else if (!topic_ch.isChannelUser(user.getFd())) {
		std::cerr << reply_.ERR_NOTONCHANNEL(arg.at(0)) << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NOTONCHANNEL(arg.at(0)));
		return;
	}
	if (arg.size() == 1) {
		queryChannelTopic(user, topic_ch);
	} else if (arg.size() == 2 && arg.at(1).size() == 1 &&
			   arg.at(1)[0] == ':') {
		removeChannelTopic(user, topic_ch);
	} else {
		setChannelTopic(user, topic_ch);
	}
}

void Command::queryChannelTopic(User &user, const Channel &topic_ch) {
	if (topic_ch.getTopicStr() == "") {
		std::cout << reply_.RPL_NOTOPIC(topic_ch.getName());
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.RPL_NOTOPIC(topic_ch.getName()));
	} else {
		std::cout << reply_.RPL_TOPIC(topic_ch.getName(),
									  topic_ch.getTopicStr());
		this->server_.sendMsgToClient(
			user.getFd(),
			reply_.RPL_TOPIC(topic_ch.getName(), topic_ch.getTopicStr()));
	}
}

void Command::removeChannelTopic(User &user, const Channel &topic_ch) {
	const_cast<Channel &>(topic_ch).setTopicStr("");
	std::cout << reply_.RPL_NOTOPIC(topic_ch.getName());
	this->server_.sendMsgToClient(user.getFd(),
								  reply_.RPL_NOTOPIC(topic_ch.getName()));
}

void Command::setChannelTopic(User &user, const Channel &topic_ch) {
	std::string topic_msg;
	// if (this->arg_.at(1)[0] == ':') {
	// 	topic_msg += this->arg_.at(1).substr(1);
	// } else {
	// 	topic_msg += this->arg_.at(1);
	// }
	for (size_t i = 1; i < this->arg_.size(); ++i) {
		if (i == 1 && this->arg_.at(i)[0] == ':') {
			topic_msg += this->arg_.at(1).substr(1);
		} else {
			topic_msg += this->arg_.at(i);
		}
		if (i < this->arg_.size() - 1) {
			topic_msg += " ";
		}
	}
	std::cout << "topic_msg: " << topic_msg << std::endl;
	const_cast<Channel &>(topic_ch).setTopicStr(topic_msg);
	std::cout << reply_.RPL_TOPIC(topic_ch.getName(), topic_ch.getTopicStr());
	this->server_.sendMsgToClient(
		user.getFd(),
		reply_.RPL_TOPIC(topic_ch.getName(), topic_ch.getTopicStr()));
}
