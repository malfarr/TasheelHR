<?php
$type = $variables['type'];
?>

<p>Dear @[name],</p>
<?php if ($type == HR_EMAIL_NOTIF_DOCUMENT_SEND_DOCUMENT) { ?>  
  <p>Kindly find attached @[document_title].</p>  
<?php } ?>
<p>
  Best Regards,<br />
  @[organization_name]
</p>
<p>
  @[responsehrm_name] Administrator
</p>

<hr/>
<p class="footer-message">
  @[responsehrm_name] is a human resource management solution that is feature-rich, intuitive and provides an essential HR management platform that consists of three main components: Personnel Information (Employee File) Management, Leave Management, and Time & Attendance Management, providing you with a user friendly, and robust solution.
</p>