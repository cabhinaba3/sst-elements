// Copyright 2009-2022 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2022, NTESS
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// of the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#ifndef _H_EMBER_TRICOUNT
#define _H_EMBER_TRICOUNT

#include "mpi/embermpigen.h"
#include <vector>
#include <queue>
#include <set>
#include <map>

namespace SST {
namespace Ember {

class EmberTriCountGenerator : public EmberMessagePassingGenerator {

public:
  SST_ELI_REGISTER_SUBCOMPONENT_DERIVED(
      EmberTriCountGenerator,
      "ember",
      "TriCountMotif",
      SST_ELI_ELEMENT_VERSION(1,0,0),
      "Performs a TriCount Motif",
      SST::Ember::EmberGenerator
      )

  SST_ELI_DOCUMENT_PARAMS(
      {   "arg.vertices_filename", "Filename for precomputed Vertices", "vertices.txt"}
      )

  EmberTriCountGenerator(SST::ComponentId_t id, Params& prms);
  ~EmberTriCountGenerator() {}
  void init_vertices();
  void first_edges();
  void starts();
  bool generate(std::queue<EmberEvent*>& evQ);
  bool task_server();
  bool task_client();
  void request_task();
  void recv_datareq();
  void wait_for_any();
  void request_edges(uint64_t vertex);
  void vertex_to_ranks(uint64_t vertex, std::set<uint64_t> &ranks);
  uint64_t vertex_to_size(uint64_t vertex, uint64_t rank);

private:
  enum {TASK_REQUEST, TASK_ASSIGN, TASK_NULL, TASKS_COMPLETE, DATA_REQUEST, DATA};
  Params params_;
  std::queue<EmberEvent*>* evQ_;
  int debug_;
  uint64_t rank_;
  uint64_t NT_;
  uint64_t NL_;
  uint64_t num_edges_;
  uint64_t num_tasks_;
  uint64_t next_task_;
  uint64_t maxTask_;
  uint64_t old_i_;
  uint64_t old_j_;
  std::vector<uint64_t> Vertices_;
  std::vector<uint64_t> first_edges_;
  std::vector<uint64_t> taskStarts_;
  unsigned int generate_loop_index_;

  // task server
  Hermes::MemAddr task_send_memaddr_;
  MessageResponse taskreq_recv_response_;
  uint64_t num_end_messages_;

  // task client
  Hermes::MemAddr task_recv_memaddr_;
//  Hermes::MemAddr datareq_send_memaddr_;
//  Hermes::MemAddr datareq_recv_memaddr_;
  std::vector<Hermes::MemAddr> datareq_send_memaddrs_;
  Hermes::MemAddr datareq_recv_memaddr_;
  MessageRequest* all_requests_;
  MessageResponse any_response_;
  MessageRequest task_recv_request_;
  MessageRequest datareq_recv_request_;
  std::vector<MessageRequest*> data_recv_requests_;
  std::vector<uint64_t> data_recv_requests_sizes_;
  bool task_recv_active_;
  bool datareq_recv_active_;
  bool need_to_wait_;
  bool free_data_requests_;
  bool free_datareq_buffers_;
  int request_index_;
  uint64_t task_index_;
  uint64_t datareq_index_;
  uint64_t num_data_ranks_;
  uint64_t num_data_received_;
  std::set<uint64_t> data_recvs_complete_;
  std::map<uint64_t, uint64_t> datareq_index_map_;
};

}
}

#endif
