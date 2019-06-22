<?php
$type = $variables['type'];
?>

<?php if ($type == HR_EMAIL_NOTIF_ANNOUNCMENT_PUBLISHED) { ?>
  <h3 class="text-center">Internal Announcement</h3>
  <p>
    <b>Subject: </b> @[topic]
  </p>
  <p>
    @[description]
  </p>  
<?php } ?>
<p>
  Sincerely,<br /><br />
  @[organization_name]<br />
  @[department]
</p>
<br /><br />
<hr/>
<p class="footer-message">
  @[responsehrm_name] is a human resource management solution that is feature-rich, intuitive and provides an essential HR management platform that consists of three main components: Personnel Information (Employee File) Management, Leave Management, and Time & Attendance Management, providing you with a user friendly, and robust solution.
</p>