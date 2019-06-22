<?php
$type = $variables['type'];
?>

<p>Dear @[name],</p>
<?php if ($type == HR_EMAIL_NOTIF_LEAVE_AUTO_APPROVAL) { ?>  
  <p>Please note that @[employee_name] has submitted a @[leave_type]. @[employee_first_name] will not attend the office as of @[leave_start_date_long].</p>  
  <div><strong>Days employee will be on leave: </strong>@[leave_date]</div>
  <div><strong>Number of days on Leave: </strong>@[leave_days]</div>
  <div><strong>Total working hours on leave: </strong>@[leave_hours]</div>
<?php } ?>
<br /><br />
<p>
  Best Regards,<br /><br />
  @[organization_name]<br />
  @[department]<br />
</p>
<br /><br />
<hr/>
<p class="footer-message">
  @[responsehrm_name] is a human resource management solution that is feature-rich, intuitive and provides an essential HR management platform that consists of three main components: Personnel Information (Employee File) Management, Leave Management, and Time & Attendance Management, providing you with a user friendly, and robust solution.
</p>