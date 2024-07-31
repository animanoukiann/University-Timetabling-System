<template>
  <div class="container">
    <div class="form-group">
      <div>
        <input v-model="courseName" placeholder="Enter name of the course" class="input-field" />
        <button @click="addCourse" class="add-button">Add Courses 📚</button>
      </div>
      <div>
        <input v-model="timeSlot" placeholder="Enter time slot" class="input-field" />
        <button @click="addTimeSlot" class="add-button">Add Time Slot⏰</button>
      </div>
      <div>
        <input v-model="instructor" placeholder="Enter instructor's name" class="input-field" />
        <button @click="addInstructor" class="add-button">Add Instructor👨‍🏫</button>
      </div>
      <div class="left-block">
        <button @click="schedule" class="add-button">Schedule📆</button>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  data() {
    return {
      courseName: '',
      timeSlot: '',
      instructor: ''
    };
  },
  methods: {
    async addCourse() {
      try {
        const response = await axios.post('http://localhost:5000/addCourse', {
          courseName: this.courseName
        });
        this.courseName = '';
        console.log(response.data);
      } catch (error) {
        console.error(error);
      }
    },
    async addTimeSlot() {
      try {
        const response = await axios.post('http://localhost:5000/addTimeslot', {
          timeSlot: this.timeSlot
        });
        this.timeSlot = '';
        console.log(response.data);
      } catch (error) {
        console.error(error);
      }
    },
    async addInstructor() {
      try {
        const response = await axios.post('http://localhost:5000/addInstructor', {
          instructor: this.instructor
        });
        this.instructor = '';
        console.log(response.data);
      } catch (error) {
        console.error(error);
      }
    },
    async schedule() {
      try {
        const response = await axios.get('http://localhost:5000/schedule');
        console.log(response.data);
      } catch (error) {
        console.error(error);
      }
    }
  }
};
</script>

<style scoped>

.container {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  min-height: 95vh;
  background-color: #ffffff69;
}

.form-group {
  position: absolute;
  top: 20px;
  left: 20px;
  background-color: #afc4ff81;
  padding: 20px;
  border-radius: 8px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.left-block {
  display: flex;
  justify-content: flex-end;
  align-items: center;
  width: 100%;
}

.input-field {
  border: 1px solid #041562;
  padding: 10px;
  border-radius: 4px;
  margin-right: 10px;
  width: 200px;
}

.add-button {
  background-color: #2945b8; /* Calm blue color */
  color: #fff;
  border: none;
  padding: 10px 20px;
  border-radius: 4px;
  cursor: pointer;
}

.add-button:hover {
  background-color: #0d00ff; /* Darker blue on hover */
}

</style>
