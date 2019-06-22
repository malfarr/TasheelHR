<?php
$type = $variables['type'];
?>

<p>Dear @[name],</p>
<?php if ($type == HR_EMAIL_NOTIF_PIM_JOB_PROBATION_END) { ?>  
  <p>Please note that the last day of @[employee_name]'s employment probationary period will be in @[days] days.</p>
  <div><strong>Job Title: </strong>@[job_title]</div>
  <div><strong>Last day of probation period: </strong>@[end_probation_date]</div>
  <div><strong>Contract start date: </strong>@[contract_start_date]</div>
  <div><strong>Direct Supervisor: </strong>@[direct_supervisor]</div>
<?php } ?>
<?php if ($type == HR_EMAIL_NOTIF_PIM_JOB_CONTRACT_END) { ?>  
  <p>Please note that the employment contract of @[employee_name] will end in @[days] days.</p>
  <div><strong>Job Title: </strong>@[job_title]</div>  
  <div><strong>Contract start date: </strong>@[contract_start_date]</div>
  <div><strong>Contract end date: </strong>@[contract_end_date]</div>
  <div><strong>Direct Supervisor: </strong>@[direct_supervisor]</div>
<?php } ?>
<br />
<br />
  <p>
  Best Regards,<br /><br />
  @[organization_name]<br />
  @[department]
</p>
<br /><br />
<hr/>
<p class="footer-message">
  @[responsehrm_name] is a human resource management solution that is feature-rich, intuitive and provides an essential HR management platform that consists of three main components: Personnel Information (Employee File) Management, Leave Management, and Time & Attendance Management, providing you with a user friendly, and robust solution.
</p>