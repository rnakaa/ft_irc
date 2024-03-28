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
	} else if (arg.size() == 2 && arg.at(1)[0] == ':') {
		removeChannelTopic(user, topic_ch);
	} else {
		// setChannelTopic();
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
	if (topic_ch.getTopicStr() == "") {
		std::cout << reply_.RPL_NOTOPIC(topic_ch.getName());
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.RPL_NOTOPIC(topic_ch.getName()));
	} else {
		const_cast<Channel &>(topic_ch).setTopicStr("");
		std::cout << reply_.RPL_TOPIC(topic_ch.getName(),
									  topic_ch.getTopicStr());
		this->server_.sendMsgToClient(
			user.getFd(),
			reply_.RPL_TOPIC(topic_ch.getName(), topic_ch.getTopicStr()));
	}
}
