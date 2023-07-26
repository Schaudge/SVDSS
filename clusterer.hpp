#ifndef CLUSTERER_HPP
#define CLUSTERER_HPP

#include <ctime>
#include <fstream>
#include <omp.h>
#include <set>
#include <stdlib.h>
#include <string>
#include <vector>

#include <htslib/hts.h>
#include <htslib/sam.h>
#include <spdlog/spdlog.h>

#include "bam.hpp"
#include "chromosomes.hpp"
#include "clipper.hpp"
#include "config.hpp"
#include "sfs.hpp"

using namespace std;

struct Cluster {
  string chrom;
  int s;
  int e;
  int cov;
  vector<SFS> SFSs;
  vector<string> names;
  vector<string> seqs;

  Cluster(){};

  Cluster(const vector<SFS> &_SFSs) {
    SFSs = _SFSs;
    chrom = _SFSs[0].chrom;
  }

  Cluster(const string &_chrom, uint _s, uint _e, uint _cov = 0) {
    chrom = _chrom;
    s = _s;
    e = _e;
    cov = _cov;
  }

  void set_coordinates(int _s, int _e) {
    s = _s;
    e = _e;
  }

  void set_cov(uint cov_) { cov = cov_; }

  void add_seq(const string &name, const string &seq) {
    names.push_back(name);
    seqs.push_back(seq);
  }

  int get_len() const {
    uint l = 0;
    uint n = 0;
    for (const string &seq : seqs) {
      ++n;
      l += seq.size();
    }
    return l / n;
  }

  string get_name(const int i) const { return names.at(i); }
  vector<string> get_names() const { return names; }
  string get_seq(const int i) const { return seqs.at(i); }

  uint size() const { return seqs.size(); }
};

class Clusterer {

private:
  Configuration *config;
  unordered_map<string, vector<SFS>> *SFSs;
  vector<SFS> extended_SFSs;
  samFile *bam_file;
  hts_idx_t *bam_index;
  bam_hdr_t *bam_header;

  // book keeping:
  uint unplaced = 0;   // SFS skipped since no first/last bases can be placed
  uint s_unplaced = 0; // SFS skipped since no first base can be placed
  uint e_unplaced = 0; // SFS skipped since no last base can be placed
  uint unknown = 0;    // SFS skipped due to (still) unknown reason
  uint unextended = 0; // SFS skipped since cannot be extended using kmers
  uint small_clusters =
      0; // number of clusters before extension  with low support
  uint small_clusters_2 =
      0; // number of clusters after extension with low support

  void align_and_extend();
  bool load_batch(int);
  void process_batch(int, int);
  void extend_alignment(bam1_t *, int);
  pair<int, int> get_unique_kmers(const vector<pair<int, int>> &alpairs,
                                  const uint k, const bool from_end,
                                  string chrom);
  void cluster_by_proximity();
  void fill_clusters();
  void store_clusters();

  // parallelize
  vector<vector<Clip>> _p_clips;
  vector<vector<SFS>> _p_extended_sfs;
  vector<vector<vector<bam1_t *>>> bam_entries;
  vector<map<pair<int, int>, vector<SFS>>> _p_sfs_clusters;

public:
  Clusterer(unordered_map<string, vector<SFS>> *);

  vector<Cluster> clusters;
  vector<Clip> clips;

  void run();
};

#endif
