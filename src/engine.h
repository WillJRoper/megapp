/*******************************************************************************
 * This file is part of MEGA++.
 * Copyright (c) 2023 Will Roper (w.roper@sussex.ac.uk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This header file contains the defintion of the Engine. The class that holds
 * all the information related to the architecture and running of MEGA++.
 ******************************************************************************/
#ifndef ENGINE_H_
#define ENGINE_H_

/* Includes */
#include <algorithm>
#include <map>
#include <regex>
#include <string>
#include <vector>

/* Local includes. */
#include "cmd_parser.h"
#include "domain.h"
#include "logging.h"
#include "params.h"
#include "threadpool.h"

/* The input data types. */
enum inputTypes { MEGA, SWIFT, FOF };

/* Make a map to convert from strings to enum. */
std::map<std::string, inputTypes> inputTypesMap{
    {"MEGA", MEGA}, {"SWIFT", SWIFT}, {"FOF", FOF}};

class Engine {
public:
  /* The run name string. */
  std::string run_name;

  /* ===================== COSMOLOGY ===================== */

  /* Read cosmology from file? */
  int read_snap_cosmo;

  /* The cosmology object. */

  /* ===================== GRAVITY ===================== */

  /* Dark matter comoving softening length. */
  double comoving_dm_soft;

  /* Dark matter max physical softening length. */
  double max_phys_dm_soft;

  /* ===================== HALO FINDING ===================== */

  /* Particle threshold for a real halo. */
  int part_threshold;

  /* Minimum particle threshold for not real halo. */
  int min_part_threshold;

  /* Spatial linking length coefficient. */
  double ll_coeff;

  /* The amount to decrement the velocity space linking length coefficient
   * each iteration. */
  double alpha_v_decrement;

  /* The initial velocity space linking length coefficient. */
  double ini_alpha_v;

  /* The minimum allowed velocity space linking length coefficient. */
  double min_alpha_v;

  /* The target overdensity for host halos. */
  int host_ovden;

  /* How many levels of substructure are we finding? */
  int n_sub_depths;

  /* The ratio in overdensity between levels in the overdensity hierarchy. */
  int sub_ratio;

  /* ===================== HALO LINKING ===================== */

  /* The number of particles in common needed for a link between halos. */
  int link_threshold;

  /* ===================== THREADPOOL ===================== */

  /* The number of threads. */
  int n_threads;

  // /* The threadpool instance. */
  ThreadPool *threadpool;

  /* ===================== INPUT ===================== */

  /* The type of input cataloge (MEGA/SWIFT/FOF). */
  enum inputTypes input_type;

  /* The file path of the directory containing inputs. */
  std::string input_dir;

  /* The input files basename. */
  std::string input_basename;

  /* How many snapshots will we be running? */
  int n_generations;

  /* The current input filepath (including directory). */
  std::string current_input;

  /* How many zeros are in the tag? */
  int tag_n_zero;

  /* ===================== LOGGING ===================== */

  /* An instance of the Logging class to report to the user. */
  Logging *log;

  /* Are we CPU profiling? */
  int cpu_prof;

  /* Are we memory profiling? */
  int mem_prof;

  /* ===================== OUTPUT ===================== */

  /* The filepath to the directory for the outputs. */
  std::string output_dir;

  /* The output file basename. */
  std::string output_basename;

  /* The current output filepath (including directory). */
  std::string current_output;

  /* The current snapshot tag of the form 00000. */
  std::string current_tag;

  /* Should we calculate halo properties? */
  int calculate_props;

  /* Should we output halo catalogues? (A file per snapshot with
   * each halo in it.) */
  int output_catalogs;

  /* Should we output the graph files? (A single file with halos sorted by
   * graph) */
  int output_graph_format;

  /* Should the unbound halos be kept in outputs? (PURELY FOR DEBUGGING) */
  int remove_not_real_halos;

  /* ===================== DOMAIN ===================== */

  /* The domain. */
  Domain *domain;

  /** @brief The constructor for the engine.
   *
   * Attaches all the members we will need during the running of MEGA++ and
   * sets up the threadpool and tasking.
   * */
  Engine(Parameters params, CommandLineParser parser, Logging log_) {

    log_.tic();

    /* First lets attach all the parameter file defined members. */

    /* Set the run name identifier. */
    run_name =
        params.getParameterString("Simulation/run_name", "SIMULATION_NO_NAME");
    log_.message("Running simulation: %s", run_name.c_str());

    /* Set the number of threads (threadpool instantiated below). */
    n_threads = parser.GetOption("--threads");
    log_.message("Running on %d threads", n_threads);

    /* Are we reading the cosmology from the snapshot? */
    read_snap_cosmo = params.getParameter("Cosmology/read_from_file", 1);
    log_.v_message("Will read cosmology from the snapshot");

    /* Set the gravitational smoothing lengths. */
    comoving_dm_soft =
        params.getParameterNoDefault<double>("Gravity/comoving_DM_softening");
    max_phys_dm_soft = params.getParameterNoDefault<double>(
        "Gravity/max_physical_DM_softening");
    log_.message("Comoving dark matter softening is %.5f [internal units]",
                 comoving_dm_soft);
    log_.message(
        "Maximum physical dark matter softening is %.5f [internal units]",
        max_phys_dm_soft);

    /* Set particle thresholds. */
    part_threshold = params.getParameter("Halos/part_threshold", 20);
    min_part_threshold = params.getParameter("Halos/min_part_threshold", 10);
    log_.message("Finding halos with more than %d particles", part_threshold);
    log_.message("Once real, halos can fall to a minimum of %d particles",
                 min_part_threshold);

    /* Set linking length values. */
    ll_coeff = params.getParameter("SpatialSearch/llcoeff", 0.2);
    log_.v_message("Spatial linking length coefficient is %.1f", ll_coeff);
    alpha_v_decrement =
        params.getParameter("PhaseSpaceSearch/decrement", 0.005);
    log_.v_message("Each phase space iteration will decrement alpha_v by %.3f",
                   alpha_v_decrement);
    ini_alpha_v = params.getParameter("PhaseSpaceSearch/ini_alpha_v", 50.0);
    log_.message("The intial alpha_v is %.1f", ini_alpha_v);
    min_alpha_v = params.getParameter("PhaseSpaceSearch/min_alpha_v", 0.08);
    log_.message("The minimum alpha_v is %.2f", min_alpha_v);

    /* Set overdensity and substructure values. */
    n_sub_depths = params.getParameter("Substructure/n_sub_depths", 1);
    sub_ratio = params.getParameter("Substructure/sub_ratio", 8);
    host_ovden = params.getParameter("Halos/host_overdensity", 200);
    log_.message("Host halos target overdensity is %d", host_ovden);
    if (n_sub_depths > 0) {
      log_.message(
          "Finding %d levels of substructure with a ratio of %d between levels",
          n_sub_depths, sub_ratio);
    }

    /* Set linking threshold. */
    link_threshold = params.getParameter("Halos/link_threshold", 10);
    log_.message("Linking halos which share at least %d particles",
                 link_threshold);

    /* Set up the input file parameters. */
    input_dir = params.getParameterString("Input/input_dir", "snapshots/");
    log_.v_message("Reading inputs from %s", input_dir.c_str());

    /* Get the input type. Simplist approach seems to be to use logic. */
    std::string input_type_str =
        params.getParameterString("Input/data_type", "SWIFT");
    if (input_type_str.at(0) == 'S') {
      input_type = SWIFT;
    } else if (input_type_str.at(0) == 'M') {
      input_type = MEGA;
    } else {
      input_type = FOF;
    }
    log_.message("Reading inputs in %s mode", input_type_str.c_str());

    // Read the input basename.
    input_basename =
        params.getParameterString("Input/input_basename", "snapshot_0000");

    /* Set the current input as the first snapshot/fof catalog. */
    std::ostringstream in_oss;
    in_oss << input_dir << "/" << input_basename << ".hdf5";
    current_input = in_oss.str();

    // Define a regex pattern to match the snapshot tag
    std::regex pattern(".*_(\\d+)");

    // Search for the snapshot tag to count the number of zeros.
    std::smatch match;
    if (std::regex_match(input_basename, match, pattern)) {
      std::string matchedNumber = match[1].str();
      tag_n_zero = matchedNumber.length();

      log_.v_message("Found snapshot tag (%s) with %d zeros",
                     matchedNumber.c_str(), tag_n_zero);
    } else {
      log_.error("Failed to find snapshot tag in %s", input_basename.c_str());
    }

    // Now strip off the snapshot tag.
    std::regex remove_pattern("(.+)_\\d*");
    if (std::regex_match(input_basename, match, remove_pattern)) {
      input_basename = match[1].str();
      input_basename += "_"; // The regex strips the trailing underscore.
    } else {
      log_.error("Failed to strip snapshot tag from %s",
                 input_basename.c_str());
    }

    /* How many snapshots will we be running over? */
    n_generations = params.getParameterNoDefault<int>("Input/snapshot_count");
    if (input_type == FOF) {
      log_.message("Running on %d FOF catalog outputs", n_generations);
    } else {
      log_.message("Running on %d snapshots", n_generations);
    }

    /* Attach the log object. */
    log = &log_;

    /* Set the profiling flags. */
    cpu_prof = params.getParameter("Profiling/cpu_profiling", 1);
    mem_prof = params.getParameter("Profiling/mem_profiling", 1);
    if (cpu_prof) {
      log_.message(
          "Will profile CPU time. Outputs will be stored in profiling/");
    }
    if (mem_prof) {
      log_.message(
          "Will profile memory usage. Outputs will be stored in profiling/");
    }
    /* Set up the output file parameters. */
    output_dir = params.getParameterString("Output/output_dir", "halos/");
    output_basename =
        params.getParameterString("Output/output_basename", "mega_halos");
    log_.message("Outputting in %s with basename %s", output_dir.c_str(),
                 output_basename.c_str());

    /* Set the current tag. */
    std::string int_str = "0";
    current_tag =
        std::string(tag_n_zero - std::min(tag_n_zero,
                                          static_cast<int>(int_str.length())),
                    '0') +
        int_str;

    /* Construct the first output string. */
    std::ostringstream out_oss;
    out_oss << output_dir << "/" << output_basename << current_tag << ".hdf5";
    current_output = out_oss.str();

    /* Set output flags. */
    calculate_props = params.getParameter("Output/calculate_props", 1);
    output_catalogs = params.getParameter("Output/output_cataloges", 1);
    output_graph_format = params.getParameter("Output/output_graph_format", 0);
    remove_not_real_halos =
        params.getParameter("Halos/remove_not_real_halos", 1);
    if (calculate_props) {
      log_.message("Will calculate halo properties and output them");
    }
    if (output_catalogs) {
      log_.message("Will output halo catalogs");
    }
    if (output_graph_format) {
      log_.message("Will output graph files");
    }
    if (!remove_not_real_halos) {
      log_.message("Will not remove unbound (not real) halos from the outputs. "
                   "THIS IS FOR DEBUGGING PURPOSES ONLY!");
    }

    /* Instantiate and attach the threadpool. */
    threadpool = new ThreadPool(n_threads);
    log->message("Instantiated the threadpool with %d threads", n_threads);

    log->toc("Initialising the Engine");
  }
  ~Engine();
};

#endif // ENGINE_H_
