#include <assert.h>
#include <stdio.h>
#include <string.h>

int name_check(char*);
int gender_check(enum gender_t);
int doctor_check(enum doctor_type_t);
int consultation_fee_check(float);
int id_check(int);
int calendar_check(char[N_DAYS][N_HOURS]);

/*
 * This function returns the object doctor_t. It reads from a file,
 * FILE* file. It determines the line which equals, int record_num.
 * Then, it returns the record located on this line.
 */

doctor_t read_doctor(FILE * file, int record_num) {
  assert(file != NULL);
  assert(record_num >= 0);

  if ((file != NULL) && (record_num >= 0)) {
    int status = 0;
    int error_stat = 0;
    int line_count = -1;
    int read_status = 0;
    doctor_t record_doc = BAD_DOCTOR;
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      line_count++;
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      if (line_count == record_num) {
        return record_doc;
      }
    } while ((status == 0) && (error_stat == 0));
  }
  return BAD_DOCTOR;
} /* read_doctor() */

/*
 * This function returns an int value confirming the status of it
 * writing to a binary file. The first parameter, FILE* file, is the
 * file the information will be written to. The second parameter,
 * doctor_t doctor, is the information that will be written to the file.
 * The third parameter, record_num, is the line that the information will
 * be written to in the file.
 */

int write_doctor(FILE * file, doctor_t doctor, int record_num) {
  assert(file != NULL);
  assert(name_check(doctor.last_name));
  assert(name_check(doctor.first_name));
  assert(id_check(doctor.id_number));
  assert(gender_check(doctor.gender));
  assert(doctor_check(doctor.doctor_type));
  assert(consultation_fee_check(doctor.consultation_fee));
  assert(calendar_check(doctor.calendar));
  assert(record_num >= 0);

  int write_status = 0;
  int read_status = 0;
  if ((file != NULL) && (record_num >= 0)) {
    int status = 0;
    int error_stat = 0;
    int line_count = -1;
    doctor_t record_doc = BAD_DOCTOR;
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      line_count++;
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      if (line_count == record_num) {
        fseek(file, record_num * sizeof(doctor), SEEK_SET);
        write_status = fwrite(&doctor, 1, sizeof(record_doc), file);
        if (write_status == 1) {
          break;
        }
        fflush(file);
        return OK;
      }
    } while ((status == 0) && (error_stat == 0));
  }

  return WRITE_ERR;
} /* write_doctor() */

/*
 * This function returns a float value of the percent of a certain
 * gender available. The first parameter, FILE* file, is where the information
 * is read from to make the calculation. The second parameter, enum gender_t
 * gender, is the gender that will be searched through the file to calculate the
 * percent.
 */

float percent_doctor_by_gender(FILE * file, enum gender_t gender) {
  int status = 0;
  int error_stat = 0;
  int total_gender = 0;
  int line_count = 0;
  doctor_t record_doc = BAD_DOCTOR;
  int read_status = 0;

  assert(file != NULL);
  assert(gender_check(gender));

  if (file != NULL) {
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      line_count++;
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      if (record_doc.gender == gender) {
        total_gender++;
      }
    } while ((status == 0) && (error_stat == 0));
  }

  line_count--;
  float percent = ((float) total_gender / line_count) * 100;
  return percent;
} /* percent_doctor_by_gender() */

/*
 * This function returns a float value of the average consultation fee
 * for a certain doctor type. The first parameter, FILE* file, is the
 * file the information is retained from. The second parameter, enum
 * doctor_type_t doctor, is the type of
 * doctor that is searched through the file to calculate the average of
 * consultation fee.
 */

float average_consultation_fee_by_type(FILE * file, enum doctor_type_t doctor) {
  int status = 0;
  int error_stat = 0;
  float total_fee = 0.0;
  int total_doctor = 0;
  doctor_t record_doc = BAD_DOCTOR;
  int read_status = 0;

  assert(file != NULL);
  assert(doctor_check(doctor));

  if (file != NULL) {
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      if (record_doc.doctor_type == doctor) {
        total_fee = total_fee + record_doc.consultation_fee;
        total_doctor++;
      }
    } while ((status == 0) && (error_stat == 0));
  }

  if (total_doctor == 0) {
    return NO_DOCTOR;
  }

  float average = 0.0;
  average = (float) total_fee / total_doctor;
  return average;
} /* average_consultation_fee_by_type() */

/*
 * This function returns a doctor_t object. The first parameter, FILE* file,
 * is the file the information is retained from. The second parameter, int id,
 * is what is searched for in the file to identify the doctor.
 */

doctor_t find_doctor_by_id(FILE * file, int id) {
  int status = 0;
  int error_stat = 0;
  doctor_t record_doc = BAD_DOCTOR;
  int read_status = 0;

  assert(file != NULL);
  assert(id_check(id));

  if (file != NULL) {
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      if (record_doc.id_number == id) {
        return record_doc;
      }
    } while ((status == 0) && (error_stat == 0));
  }

  return BAD_DOCTOR;
} /* find_doctor_by_id() */

/*
 * This function returns the doctor_t value. It searches
 * through a file given the name and doctor type, and returns
 * the doctor that satisfies these requirements. The first
 * parameter, FILE* file, is the file the information is taken from.
 * THe second parameter, char* first_name, and the third parameter, char*
 * last_name is the name of the doctor that will be seached through the file.
 * The fourth parameter, enum doctor_type_t doctor_type, is the doctor_type
 * that will be looked upon when the doctor of the name is found.
 */

doctor_t find_doctor_by_name(FILE * file, char * first_name, char * last_name,
enum doctor_type_t doctor_type) {
  int status = 0;
  int error_stat = 0;
  doctor_t record_doc = BAD_DOCTOR;
  doctor_t doctors[100];

  assert(file != NULL);
  assert(name_check(first_name));
  assert(name_check(last_name));
  assert(doctor_check(doctor_type));

  int doctor_index = 0;
  int read_status = 0;

  if (file != NULL) {
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      if (strcmp(first_name, record_doc.first_name) == 0) {
        if (strcmp(last_name, record_doc.last_name) == 0) {
          if (doctor_type == record_doc.doctor_type) {
            doctors[doctor_index] = record_doc;
            doctor_index++;
          }
        }
      }
    } while ((status == 0) && (error_stat == 0));
  }

  int min_id = 0;
  for (int i = 1; i < doctor_index; i++) {
    if (doctors[min_id].id_number > doctors[i].id_number) {
      min_id = i;
    }
  }

  if (doctor_index == 0) {
    return BAD_DOCTOR;
  }

  return doctors[min_id];
} /* find_doctor_by_name() */

/*
 * This function returns an int value that confirms whether or not
 * the update is written back to the file. The first parameter is the
 * file that is read through to find the doctor with the id given as
 * the second parameter. Then the consultation fee of this doctor is
 * changed to the value given as the third parameter, float change_fee
 */

int update_doctor_consultation_fee(FILE * file, int id, float change_fee) {
  int status = 0;
  int error_stat = 0;
  doctor_t record_doc = BAD_DOCTOR;
  doctor_t temp_doctor = BAD_DOCTOR;
  int line_count = 0;
  int read_status = 0;

  assert(file != NULL);
  assert(id_check(id));

  int current_position = 0;
  int write_status = 0;

  if (file != NULL) {
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      line_count++;
      if (record_doc.id_number == id) {
        if (change_fee > 0) {
          current_position = ftell(file);
          record_doc.consultation_fee = change_fee;
          temp_doctor = record_doc;
          fseek(file, current_position - sizeof(record_doc), SEEK_SET);
          write_status = fwrite(&temp_doctor, 1, sizeof(record_doc), file);
          if (write_status == 0) {
            break;
          }
          fflush(file);
        }
        return OK;
      }
    } while ((status == 0) && (error_stat == 0));
  }

  return NO_DOCTOR;
} /* update_doctor_consultation_fee() */

/*
 * This function returns the float value of the total
 * consultation fee. This is calculated through finding a doctor
 * and using the number of worked hours and multiplying it with
 * the consultation fee. The first parameter, FILE* file, is the file
 * the information is read from. The second parameter, int id, is the
 * id of the doctor that will be identified in the file.
 */

float calculate_total_consultation_fee(FILE * file, int id) {
  int status = 0;
  int error_stat = 0;
  doctor_t record_doc = BAD_DOCTOR;
  float total_fee = 0.0;
  float consultation_fee = 0.0;
  int total_hours = 0;
  int total_doc = 0;
  int read_status = 0;

  assert(file != NULL);
  assert(id_check(id));

  if (file != NULL) {
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      if (record_doc.id_number == id) {
        for (int i = 0; i < N_DAYS; i++) {
          for (int j = 0; j < N_HOURS; j++) {
            if (record_doc.calendar[i][j] == 'B') {
              total_hours++;
            }
          }
        }
        consultation_fee = record_doc.consultation_fee;
        total_doc++;
      }
    } while ((status == 0) && (error_stat == 0));
  }

  total_fee = (float)total_hours * consultation_fee;
  if (total_doc == 0) {
    return NO_DOCTOR;
  }
  return total_fee;
} /* calculate_total_consultation_fee() */

/*
 * This function returns an int value to confirm whther or not
 * an appointment has been made. The first parameter, FILE* file,
 * is where all the information is read from. The second parameter,
 * int id, identifies a specific record in the file. The third
 * and fourth parameter, int day and int hour, is the time that is
 * being requested to book an apoointment.
 */

int make_appointment(FILE * file, int id, int day, int hour) {
  int status = 0;
  int error_stat = 0;
  int doctor_count = -1;
  int line_index = 0;
  doctor_t record_doc = BAD_DOCTOR;
  doctor_t temp_doctor = BAD_DOCTOR;
  int read_status = 0;
  int write_status = 0;

  assert(file != NULL);
  assert(id_check(id));

  if ((day < 1) || (day > 5)) {
    return BAD_APPOINTMENT;
  }

  if ((hour < 8) || (hour > 16)) {
    return BAD_APPOINTMENT;
  }

  hour = hour - 8;

  int current_position = 0;
  if (file != NULL) {
    fseek(file, 0, SEEK_SET);
    do {
      read_status = fread(&record_doc, sizeof(record_doc), 1, file);
      status = feof(file);
      error_stat = ferror(file);
      if ((status != 0) || (error_stat != 0) || (read_status != 1)) {
        break;
      }
      line_index++;
      if (record_doc.id_number == id) {
        if (record_doc.calendar[day - 1][hour] == 'B') {
          return UNAVAILABLE;
        }
        else {
          record_doc.calendar[day - 1][hour] = 'B';
          temp_doctor = record_doc;
          current_position = ftell(file);
          fseek(file, current_position - sizeof(record_doc), SEEK_SET);
          write_status = fwrite(&temp_doctor, sizeof(record_doc), 1, file);
          if (write_status != 1) {
            break;
          }
          fflush(file);
        }
        doctor_count++;
      }
    } while ((status == 0) && (error_stat == 0));
  }

  if (doctor_count == -1) {
    return NO_DOCTOR;
  }
  return OK;

} /* make_appointment() */

/*
 * This function returns an int that verfies if the
 * given parameter, char* name, is valid or not
 */

int name_check(char* name) {
  int length = 0;
  length = sizeof(name);
  int count = 0;
  for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (name[i] == '\0') {
      count++;
    }
  }
  if (name == NULL) {
    return 0;
  }
  if (length >= MAX_NAME_LEN) {
    return 0;
  }
  if (count == 0) {
    return 0;
  }
  return 1;
} /* name_check() */

/*
 * This function returns an int value that verifies
 * if the given parameter, enum gender_t gender, is valid
 * or not.
 */

int gender_check(enum gender_t gender) {
  if ((gender >= 0) && (gender <= 2)) {
    return 1;
  }
  return 0;
} /* gender_check() */

/*
 * This function returns an int value that verifies if
 * the given parameter, enum doctor_type_t doctor_type,
 * is valid or not.
 */

int doctor_check(enum doctor_type_t doctor_type) {
  if ((doctor_type >= 0) && (doctor_type <= 3)) {
    return 1;
  }
  return 0;
} /* doctor_check() */

/*
 * This function returns an int value that verifies if
 * the given parameter, float fee, is valid or not.
 */

int consultation_fee_check(float fee) {
  if (fee > 0) {
    return 1;
  }
  return 0;
} /* consultation_fee_check() */

/*
 * This function returns an int value that verifies if the given
 * parameter, char calender[N_DAYS][N_HOURS], is valid or not.
 */

int calendar_check(char calendar[N_DAYS][N_HOURS]) {
  int error = 0;
  for (int i = 0; i < N_DAYS; i++) {
    for (int j = 0; j < N_HOURS; j++) {
      if ((calendar[i][j] != 'A') && (calendar[i][j] != 'B')) {
        error++;
      }
    }
  }

  if (error == 0) {
    return 1;
  }

  return 0;
} /* calendar_check() */

/*
 * This function returns an int value that verifies if the
 * given parameter, int id, is valid or not.
 */

int id_check(int id) {
  if (id > 0) {
    return 1;
  }
  return 0;
} /* id_check() */
