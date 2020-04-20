#include <mpi.h>
#include <array>
#include <iostream>

using namespace std;

void second_task(MPI_Group global_group, MPI_Comm global_communitator, int rank, int size) {
	MPI_Group limited_group;
	MPI_Comm limited_communicator, duplicated_communicator;
	array<int, 6> group_ranks = { 0, 2, 3, 4, 6, 7 };
	MPI_Group_incl(global_group, group_ranks.size(), group_ranks.data(), &limited_group);
	MPI_Comm_create(global_communitator, limited_group, &limited_communicator);
	
	int data_for_communication;
	int rank_in_group;
	
	MPI_Group_rank(limited_group, &rank_in_group);
	if (rank_in_group >= 0) {
		MPI_Comm_dup(limited_communicator, &duplicated_communicator);
		if (rank_in_group == 2) {
			data_for_communication = 1234;
			cout << "RANK " << rank_in_group << " SET DATA TO " << data_for_communication;
		}
		MPI_Bcast(&data_for_communication, sizeof(data_for_communication), MPI_INT, 2, duplicated_communicator);
		if (rank_in_group % 3 == 0) {
			cout << "RANK " << rank_in_group << " RECEIVED " << data_for_communication;
		}
		MPI_Group_free(&limited_group);
		MPI_Comm_free(&duplicated_communicator);
		MPI_Comm_free(&limited_communicator);
	}

}

int main(int argc, char *argv[]) {
	MPI_Comm global_communicator;
	MPI_Group global_group;
	int world_rank, world_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_dup(MPI_COMM_WORLD, &global_communicator);
	MPI_Comm_group(global_communicator, &global_group);
	if (world_size >= 8) {
		second_task(global_group, global_communicator, world_rank, world_size);
	}
	MPI_Group_free(&global_group);
	MPI_Comm_free(&global_communicator);
	MPI_Finalize();
}
