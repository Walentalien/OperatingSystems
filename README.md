# OperatingSystems
My solutions for assignments from Operating Systems class



## Assignment for 10th class contents
In a small hospital, there is one doctor on duty who serves two groups:

**Patients** (e.g. 20 - the number of patients should be configurable) who come for a consultation,

**Pharmacists** (e.g. 3 - the number of pharmacists should be configurable) who deliver medicines to the pharmacy in the hospital.


The doctor is a very busy and overworked man, so when he goes to bed he sleeps as long as he can. 
The doctor, as mentioned, is busy, so he only **performs a consultation if exactly 3 patients gather**. After the consultation, the doctor goes back to sleep. Due to legally imposed restrictions after pandemi, a maximum of 3 patients are allowed in the hospital, and the rest are not allowed in at all.

The first aid kit (medicines) has a limited capacity (e.g., 6), and each consultation consumes 3 units of medicine (one for each patient at the consultation). Replenishment of the pharmacy is possible thanks to pharmacists.
A doctor can accept patients only if he has enough medicine available in the medicine cabinet (at least 3).


### Comminication Logging
Doctor, patients and pharmacists should inform in the form of messages (information to the standard output about current stages). Variables for messages: TIME - exact time in the system, PATIENT_ID - patient ID, PHARMACIST_ID - pharmacist ID.

### Patient behaviors:
They arrive at the hospital after a random time (2–5s).
(Message: [TIME] - Patient(PATIENT_ID): I'm going to the hospital, I will be there in _ s).

If there are already 3 patients in the waiting room – they come back later (they take a short walk around the hospital and check the waiting room again; if it's still full, they walk again).
(Message: [TIME] - Patient(PATIENT_ID): too many patients, I’ll come back later in _ s).

Otherwise, they sit in the waiting room.
(Message: [TIME] - Patient(PATIENT_ID): _ patients waiting for the doctor).

If they are the third patient in the waiting room, they wake the doctor.
(Message: [TIME] - Patient(PATIENT_ID): waking up the doctor).

After the consultation, they go home.
(Message: [TIME] - Patient(PATIENT_ID): finishing the visit).

### Pharmacist behaviors:
They arrive with a new delivery after a random time (5–15s).
(Message: [TIME] - Pharmacist(PHARMACIST_ID): I'm going to the hospital, I will be there in _ s).

If the medicine cabinet is full – they wait for it to be emptied.
(Message: [TIME] - Pharmacist(PHARMACIST_ID): waiting for the medicine cabinet to be emptied).

If the medicine cabinet is empty or running low (number of medicines < 3):

They wake up the doctor.
(Message: [TIME] - Pharmacist(PHARMACIST_ID): waking up the doctor).

They deliver a new batch of medicine.
(Message: [TIME] - Pharmacist(PHARMACIST_ID): delivering medicine).

After the delivery, they finish their work.
(Message: [TIME] - Pharmacist(PHARMACIST_ID): delivery complete).


### Doctor behaviors:
Sleeps until one of the following conditions is met:

- 3 patients are waiting, and there are at least 3 medicines available.
- A pharmacist is waiting, and the medicine cabinet is running low (medicines < 3).

Wakes up.
(Message: [TIME] - Doctor: waking up).

If there are patients and enough medicines:

- Consults them.
  (Message: [TIME] - Doctor: consulting patients PATIENT_ID1, PATIENT_ID2, PATIENT_ID3),
- Uses 3 medicines,
- The consultation takes 2–4s.

Otherwise, if there are pharmacists and space in the medicine cabinet:

- Accepts the delivery.
  (Message: [TIME] - Doctor: receiving a medicine delivery),
- Refills the medicine cabinet,
- The delivery process takes 1–3s.

After completing the action, the doctor goes back to sleep.
(Message: [TIME] - Doctor: going back to sleep).


## Additional task specification:
The task also said to implement the program using threads (pthread) and synchronization mechanisms of the POSIX Threads library (mutex, condition variable). 
When the program is started, the main thread creates threads for the Doctor, Patients and Pharmacists. 
The number of patients and pharmacists should be passable as a run parameter of the program (the first parameter is the number of patients, the second parameter is the number of pharmacists). 
The doctor's work ends when there are no more patients to treat. Check Conditions (Condition Variables) should be used to bed down the Doctor.

The use of appropriate mechanisms is to guarantee underdelivery, for example, to events: The doctor is sleeping even though there are 3 patients waiting for him, or the Pharmacist is unable to refill medicines, more than one pharmacist refills the medicine cabinet, etc.

### 2 Versions of the problem:
The task can be given in 2 versions:

100% of the grade  - full solution as described above (doctor + patients and pharmacists)
60% of the grade - partial solution (doctor + patients - no pharmacists and no first aid kit)

## My comment
In this simulation i assumed that patients need to go to doctor avery once in a while and never get cured;

The simulation can be expanded:
There will be a chance of a patient to get cured;
And If there is less than 3 patients who need a doctor, ehat the last patient comes to a waiting room and there is lesss than 2 patientz he can askdoctor niely to threat them anyway

How many patiens allowed in waiting Room
Give different priority to patients in the waiting room