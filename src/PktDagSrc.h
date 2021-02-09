// See the file "COPYING" in the main distribution directory for copyright.
//
// Support for Endace's DAG interface card.
//
// Caveats:
//    - No support for hardware-side filtering yet.
//    - No support for snaplen yet.
//    - No support for other media than Ethernet.
//    - Currently pretends fd is selectable to work around bro limitations

#ifndef PKTDAGSRC_H
#define PKTDAGSRC_H

extern int snaplen;

#include "iosource/PktSrc.h"
#include "dag_config_api.h"

#if ZEEK_VERSION_NUMBER >= 40100
using namespace zeek;
#endif

namespace Endace_DAG {

class PktDagSrc : public iosource::PktSrc {
public:
	PktDagSrc(const std::string& path, bool is_live);
	virtual ~PktDagSrc();
	static PktSrc* InstantiatePktDagSrc(const std::string& path, bool is_live);

	// PktSrc interface:
	virtual void Statistics(Stats* stats);
	virtual bool PrecompileFilter(int index, const std::string& filter);
	virtual bool SetFilter(int index);

protected:
	virtual void Open();
	virtual bool ExtractNextPacket(Packet* pkt);
	virtual void DoneWithPacket();
	//virtual void GetFds(int* read, int* write, int* except);
	virtual void Close();

private:
	static const unsigned int EXTRA_WINDOW_SIZE = 4 * 1024 * 1024;
	int stream_num;

	dag_card_ref_t dag_ref;
	dag_component_t dag_root;
	attr_uuid_t drop_attr;
	int fd;
	int current_filter;

	Properties props;
	Stats stats;
};

}

#endif
